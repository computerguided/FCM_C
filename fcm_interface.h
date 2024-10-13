// -------------------------------------------------------------------------------------------------
// fcm_interface.h
// -------------------------------------------------------------------------------------------------

#ifndef FCM_INTERFACE_H
#define FCM_INTERFACE_H

#include "fcm_types.h"

typedef struct
{
	char* p_name;
	void* p_remote_interface;
	void* p_component;
} fcm_interface_t;

// -------------------------------------------------------------------------------------------------
#define FCM_INTERFACE_BASETYPE_FIELDS \
		char* p_name; \
		void* p_remote_interface; \
		void* p_component

// -------------------------------------------------------------------------------------------------
// see the remark in fcm_message_queue.h, message structs *must* be 8 aligned!
#define FCM_MESSAGE_DEF(m,...) \
		struct \
		{ \
			char* p_message_id __attribute__ ((aligned(4))); \
			struct \
			{ \
			__VA_ARGS__ \
			} __attribute__ ((aligned(8))); \
		} __attribute__ ((packed)) * m; \
		char* p_##m##_id

// -------------------------------------------------------------------------------------------------
#define FCM_SET_MESSAGE_ID(m) \
	    _Static_assert(sizeof(*p_interface->m) % 8u == 4, "Message must be sized a multiple of 8! (minus sizeof(p_message_id))"); \
		p_interface->p_##m##_id = #m ;

// -------------------------------------------------------------------------------------------------
#define FCM_INTERFACE_INIT_FUNCTION(name) \
		void name##_init( name##_t* p_interface)

// -------------------------------------------------------------------------------------------------
void fcm_connect_interfaces(fcm_interface_t* p_left, fcm_interface_t* p_right);



#endif




