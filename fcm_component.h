// -------------------------------------------------------------------------------------------------
// fcm_component.h
//
// -------------------------------------------------------------------------------------------------

#ifndef FCM_COMPONENT_H
#define FCM_COMPONENT_H

#include <assert.h>

#include "fcm_logical.h"
#include "fcm_message_queue.h"
#include "fcm_state_transition_table.h"
#include "fcm_timer.h"
#include "fcm_types.h"

// -------------------------------------------------------------------------------------------------
// Logging
// -------------------------------------------------------------------------------------------------
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "hardfault_genhf.h"

// -------------------------------------------------------------------------------------------------
// Component basetype
// -------------------------------------------------------------------------------------------------

typedef struct
{
	char*                p_name;
	fcm_message_queue_t* p_message_queue;
	fcm_stt_element_t*   p_current_state;
	fcm_stt_element_t*   p_transitions;
	fcm_logical_t        logical;
	fcm_timer_t          timer;
} fcm_component_t;

#define FCM_COMPONENT_BASETYPE_FIELDS \
		char*                p_name; \
		fcm_message_queue_t* p_message_queue; \
		fcm_stt_element_t*   p_current_state; \
		fcm_stt_element_t*   p_transitions; \
		fcm_logical_t        logical; \
		fcm_timer_t          timer

// -------------------------------------------------------------------------------------------------
typedef void (*fcm_transition_function_t)(fcm_component_t*);
typedef bool (*fcm_evaluation_function_t)(fcm_component_t*);

// -------------------------------------------------------------------------------------------------
typedef struct
{
	char* p_name;
	fcm_evaluation_function_t p_evaluation; // Used if state is a choice-point, also as indicator.
} fcm_state_t;

#define FCM_COMPONENT_STATE(name) \
        fcm_state_t name

#define FCM_COMPONENT_CHOICE_POINT(name) \
        fcm_state_t name

// -------------------------------------------------------------------------------------------------
void fcm_set_first_state(fcm_component_t* p_component, fcm_state_t* p_state);

// -------------------------------------------------------------------------------------------------
#define FCM_TRANSITION_PRINT(...) \
	NRF_LOG_INFO(__VA_ARGS__) \
	NRF_LOG_FLUSH()

// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// Add interface in component header file.
// -------------------------------------------------------------------------------------------------
#define FCM_COMPONENT_INTERFACE(name) \
        name##_t name

// -------------------------------------------------------------------------------------------------
// Macros in the component initialization.
// -------------------------------------------------------------------------------------------------

#define FCM_COMPONENT_INIT_FUNCTION(name) \
		void name##_init(FCM_COMPONENT_TYPE* p_component, fcm_message_queue_t* p_message_queue)

#define FCM_COMPONENT_INIT_BASE(name) \
		p_component->p_name = #name; \
		FCM_SET_INTERFACE(logical)

#define FCM_COMPONENT_INIT_FINISHED \
	    NRF_LOG_INFO("%s initialized", p_component->p_name); \
	    NRF_LOG_FLUSH()

#define FCM_SET_STATE(s) \
		p_component->s.p_name = #s; \
		p_component->s.p_evaluation = NULL

#define FCM_SET_CHOICEPOINT(s) \
		p_component->s.p_name = #s"?"; \
		p_component->s.p_evaluation = s##_wrapper

#define FCM_SET_INTERFACE(i) \
		i##_init(&p_component->i); \
		p_component->i.p_component = p_component

#define FCM_INIT_STT(x,s) \
		static fcm_stt_element_t stt_elements[x*5]; \
		fcm_set_stt_elements(stt_elements,x*5); \
		p_component->p_transitions = stt_elements; \
		FCM_SET_TRANSITIONS(x); \
		fcm_set_next_states(p_component->p_transitions,x*5); \
		fcm_set_first_state((fcm_component_t*)p_component,&p_component->s)

#define FCM_SET_MESSAGE_QUEUE(m) \
		p_component->p_message_queue = m

#define FCM_COMPONENT(c) \
        c##_t c

// -------------------------------------------------------------------------------------------------
// Macros to be used for/in transition function.
// -------------------------------------------------------------------------------------------------

#define FCM_TRANSITION_FUNCTION(x,s,i,m,n) \
		static void _##s##_##i##_##m##_wrapper(fcm_component_t* p_component); \
		static void _trans_##x(FCM_COMPONENT_TYPE* p_component) \
		{ \
			fcm_set_transition( \
					x, \
					p_component->p_transitions, \
					FCM_NUM_TRANSITIONS*5, \
					&p_component->s, \
					&p_component->i, \
					p_component->i.p_##m##_id, \
					_##s##_##i##_##m##_wrapper, \
					&p_component->n ); \
		} \
		static void _##s##_##i##_##m(FCM_COMPONENT_TYPE* p_component); \
		static void _##s##_##i##_##m##_wrapper(fcm_component_t* p_generic_component) \
		{ \
             FCM_COMPONENT_TYPE* p_component = (FCM_COMPONENT_TYPE*)p_generic_component; \
             \
			 if ((void*)&(p_component->i) == (void*)&(p_component->logical)) \
             { \
				 /* messages on the logical interface aren't queued */ \
				 p_component->i.m = (__typeof__(p_component->i.m))&p_component->i.p_##m##_id; \
             } \
             else \
			 { \
		         p_component->i.m = (__typeof__(p_component->i.m))&p_component->p_message_queue->p_read->p_message_id; \
		         \
		         /* add the size of p_message_id (which is part of fcm_message_t 8 aligned struct) */ \
		         /* to get the address of the first message parameter member */ \
		         /* that address *must* be 8 aligned */ \
			     /* except if the message has no parameters */ \
	             if ((sizeof(*(p_component->i.m)) > sizeof(char*)) && \
	    	        	 !MESSAGE_IS_ALIGNED((uint8_t*)(p_component->i.m) + sizeof(char*))) \
	             { \
 		             NRF_LOG_INFO("Fatal error: message %s on interface %s misaligned @%ld, triggering hardfault!", #m, #i, p_component->i.m); \
		             NRF_LOG_FLUSH(); \
		             /* trigger hardfault rather than continue with uncertain behavior */ \
		             hardfault_genhf_ldm_align(); \
	             } \
	         } \
			 FCM_TRANSITION_PRINT("Transition: %s | %s | %s | %s", p_component->p_name, p_component->s.p_name, p_component->i.p_name, p_component->i.m->p_message_id); \
			  _##s##_##i##_##m(p_component); \
		     p_component->i.m = NULL; \
		} \
		static void _##s##_##i##_##m(FCM_COMPONENT_TYPE* p_component)

// -------------------------------------------------------------------------------------------------

#define FCM_EVALUATION_FUNCTION(n) \
		static bool n(FCM_COMPONENT_TYPE* p_component); \
		static bool n##_wrapper(fcm_component_t* p_component) \
		{ \
			return n((FCM_COMPONENT_TYPE*)p_component); \
		} \
		static bool n(FCM_COMPONENT_TYPE* p_component)

// -------------------------------------------------------------------------------------------------

#define FCM_MESSAGE_CHECK(i,m) \
		assert( p_component->i.m != NULL )

#define FCM_PREPARE_MESSAGE(i,m) \
		p_component->i.m = fcm_prepare_message(p_component->p_message_queue, p_component->i.p_##m##_id, sizeof(*p_component->i.m));

#define FCM_SET_MESSAGE_PARAMETER(i,m,p,v) \
		p_component->i.m->p = v

#define FCM_MESSAGE_PARAMETER_VALUE(i,m,p) \
        p_component->i.m->p

#define FCM_SEND_MESSAGE(i,m) \
		fcm_send_message(p_component->p_message_queue, (fcm_interface_t*)&p_component->i); \
		p_component->i.m = NULL;

// -------------------------------------------------------------------------------------------------

#define FCM_SET_TRANS_1 _trans_1(p_component)
#define FCM_SET_TRANS_2 FCM_SET_TRANS_1; _trans_2(p_component)
#define FCM_SET_TRANS_3 FCM_SET_TRANS_2; _trans_3(p_component)
#define FCM_SET_TRANS_4 FCM_SET_TRANS_3; _trans_4(p_component)
#define FCM_SET_TRANS_5 FCM_SET_TRANS_4; _trans_5(p_component)
#define FCM_SET_TRANS_6 FCM_SET_TRANS_5; _trans_6(p_component)
#define FCM_SET_TRANS_7 FCM_SET_TRANS_6; _trans_7(p_component)
#define FCM_SET_TRANS_8 FCM_SET_TRANS_7; _trans_8(p_component)
#define FCM_SET_TRANS_9 FCM_SET_TRANS_8; _trans_9(p_component)
#define FCM_SET_TRANS_10 FCM_SET_TRANS_9; _trans_10(p_component)
#define FCM_SET_TRANS_11 FCM_SET_TRANS_10; _trans_11(p_component)
#define FCM_SET_TRANS_12 FCM_SET_TRANS_11; _trans_12(p_component)
#define FCM_SET_TRANS_13 FCM_SET_TRANS_12; _trans_13(p_component)
#define FCM_SET_TRANS_14 FCM_SET_TRANS_13; _trans_14(p_component)
#define FCM_SET_TRANS_15 FCM_SET_TRANS_14; _trans_15(p_component)
#define FCM_SET_TRANS_16 FCM_SET_TRANS_15; _trans_16(p_component)
#define FCM_SET_TRANS_17 FCM_SET_TRANS_16; _trans_17(p_component)
#define FCM_SET_TRANS_18 FCM_SET_TRANS_17; _trans_18(p_component)
#define FCM_SET_TRANS_19 FCM_SET_TRANS_18; _trans_19(p_component)
#define FCM_SET_TRANS_20 FCM_SET_TRANS_19; _trans_20(p_component)
#define FCM_SET_TRANS_21 FCM_SET_TRANS_20; _trans_21(p_component)
#define FCM_SET_TRANS_22 FCM_SET_TRANS_21; _trans_22(p_component)
#define FCM_SET_TRANS_23 FCM_SET_TRANS_22; _trans_23(p_component)
#define FCM_SET_TRANS_24 FCM_SET_TRANS_23; _trans_24(p_component)
#define FCM_SET_TRANS_25 FCM_SET_TRANS_24; _trans_25(p_component)
#define FCM_SET_TRANS_26 FCM_SET_TRANS_25; _trans_26(p_component)
#define FCM_SET_TRANS_27 FCM_SET_TRANS_26; _trans_27(p_component)
#define FCM_SET_TRANS_28 FCM_SET_TRANS_27; _trans_28(p_component)
#define FCM_SET_TRANS_29 FCM_SET_TRANS_28; _trans_29(p_component)
#define FCM_SET_TRANS_30 FCM_SET_TRANS_29; _trans_30(p_component)
#define FCM_SET_TRANS_31 FCM_SET_TRANS_30; _trans_31(p_component)
#define FCM_SET_TRANS_32 FCM_SET_TRANS_31; _trans_32(p_component)
#define FCM_SET_TRANS_33 FCM_SET_TRANS_32; _trans_33(p_component)
#define FCM_SET_TRANS_34 FCM_SET_TRANS_33; _trans_34(p_component)
#define FCM_SET_TRANS_35 FCM_SET_TRANS_34; _trans_35(p_component)
#define FCM_SET_TRANS_36 FCM_SET_TRANS_35; _trans_36(p_component)
#define FCM_SET_TRANS_37 FCM_SET_TRANS_36; _trans_37(p_component)
#define FCM_SET_TRANS_38 FCM_SET_TRANS_37; _trans_38(p_component)
#define FCM_SET_TRANS_39 FCM_SET_TRANS_38; _trans_39(p_component)
#define FCM_SET_TRANS_40 FCM_SET_TRANS_39; _trans_40(p_component)
#define FCM_SET_TRANS_41 FCM_SET_TRANS_40; _trans_41(p_component)
#define FCM_SET_TRANS_42 FCM_SET_TRANS_41; _trans_42(p_component)
#define FCM_SET_TRANS_43 FCM_SET_TRANS_42; _trans_43(p_component)
#define FCM_SET_TRANS_44 FCM_SET_TRANS_43; _trans_44(p_component)
#define FCM_SET_TRANS_45 FCM_SET_TRANS_44; _trans_45(p_component)
#define FCM_SET_TRANS_46 FCM_SET_TRANS_45; _trans_46(p_component)
#define FCM_SET_TRANS_47 FCM_SET_TRANS_46; _trans_47(p_component)
#define FCM_SET_TRANS_48 FCM_SET_TRANS_47; _trans_48(p_component)
#define FCM_SET_TRANS_49 FCM_SET_TRANS_48; _trans_49(p_component)
#define FCM_SET_TRANS_50 FCM_SET_TRANS_49; _trans_50(p_component)
#define FCM_SET_TRANS_51 FCM_SET_TRANS_50; _trans_51(p_component)
#define FCM_SET_TRANS_52 FCM_SET_TRANS_51; _trans_52(p_component)
#define FCM_SET_TRANS_53 FCM_SET_TRANS_52; _trans_53(p_component)
#define FCM_SET_TRANS_54 FCM_SET_TRANS_53; _trans_54(p_component)
#define FCM_SET_TRANS_55 FCM_SET_TRANS_54; _trans_55(p_component)
#define FCM_SET_TRANS_56 FCM_SET_TRANS_55; _trans_56(p_component)
#define FCM_SET_TRANS_57 FCM_SET_TRANS_56; _trans_57(p_component)
#define FCM_SET_TRANS_58 FCM_SET_TRANS_57; _trans_58(p_component)
#define FCM_SET_TRANS_59 FCM_SET_TRANS_58; _trans_59(p_component)
#define FCM_SET_TRANS_60 FCM_SET_TRANS_59; _trans_60(p_component)
#define FCM_SET_TRANS_61 FCM_SET_TRANS_60; _trans_61(p_component)
#define FCM_SET_TRANS_62 FCM_SET_TRANS_61; _trans_62(p_component)
#define FCM_SET_TRANS_63 FCM_SET_TRANS_62; _trans_63(p_component)
#define FCM_SET_TRANS_64 FCM_SET_TRANS_63; _trans_64(p_component)
#define FCM_SET_TRANS_65 FCM_SET_TRANS_64; _trans_65(p_component)
#define FCM_SET_TRANS_66 FCM_SET_TRANS_65; _trans_66(p_component)
#define FCM_SET_TRANS_67 FCM_SET_TRANS_66; _trans_67(p_component)
#define FCM_SET_TRANS_68 FCM_SET_TRANS_67; _trans_68(p_component)
#define FCM_SET_TRANS_69 FCM_SET_TRANS_68; _trans_69(p_component)
#define FCM_SET_TRANS_70 FCM_SET_TRANS_69; _trans_70(p_component)
// Extend this when more than 70 transitions.

#define FCM_SET_TRANSITIONS(x) \
		FCM_SET_TRANS_##x

#endif
