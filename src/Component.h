// -------------------------------------------------------------------------------------------------
// Component.h
//
// -------------------------------------------------------------------------------------------------

#ifndef COMPONENT_H
#define COMPONENT_H

#include <assert.h>

#include "Types.h"
#include "StateTransitionTable.h"
#include "MessageQueue.h"
#include "Logical.h"
#include "Timer.h"

// -------------------------------------------------------------------------------------------------
// Component basetype
// -------------------------------------------------------------------------------------------------

typedef struct
{
	MessageQueue_t* pMsgQueue;
	SttElement_t* pCurrentState;
	SttElement_t* pTransitions;
	Logical_t Logical;
	Timer_t Timer;
} Component_t;

#define COMPONENT_BASETYPE_FIELDS \
		MessageQueue_t* pMsgQueue; \
		SttElement_t* pCurrentState; \
		SttElement_t* pTransitions; \
		Logical_t Logical; \
		Timer_t Timer

// -------------------------------------------------------------------------------------------------
typedef void (*TransitionFunction_t)(Component_t*);
typedef bool (*EvaluationFunction_t)(Component_t*);

// -------------------------------------------------------------------------------------------------
typedef struct
{
	char* pName;
	EvaluationFunction_t pEvaluation; // Used if state is a choicepoint, also as indicator.
} State_t;

// -------------------------------------------------------------------------------------------------
void SetFirstState(Component_t* pComp, State_t* pState);

// -------------------------------------------------------------------------------------------------
// Macros in the component initialization.
// -------------------------------------------------------------------------------------------------

#define SET_STATE(s) \
		pComp->s.pName = #s; \
		pComp->s.pEvaluation = NULL

#define SET_CHOICEPOINT(s) \
		pComp->s.pName = #s"?"; \
		pComp->s.pEvaluation = s##_wrapper

#define SET_INTERFACE(i) \
		i##_init(&pComp->i); \
		pComp->i.pComponent = pComp

#define INIT_STT(x,s) \
		SET_TRANSITIONS(x); \
		SetNextStates(pComp->pTransitions); \
		SetFirstState((Component_t*)pComp,&pComp->s)

#define SET_MSG_QUEUE(m) \
		pComp->pMsgQueue = m

// -------------------------------------------------------------------------------------------------
// Macros to be used for/in transition function.
// -------------------------------------------------------------------------------------------------

#define TRANSITION_FUNCTION(x,s,i,m,n) \
		static void _##s##_##i##_##m##_wrapper(Component_t* pComp); \
		static void _trans_##x(COMPONENT_TYPE* pComp) \
		{ \
			pComp->pTransitions = SetTransition( \
					x, \
					pComp->pTransitions, \
					&pComp->s, \
					&pComp->i, \
					pComp->i.p##m##_id, \
					_##s##_##i##_##m##_wrapper, \
					&pComp->n ); \
		} \
		static void _##s##_##i##_##m(COMPONENT_TYPE* pComp); \
		static void _##s##_##i##_##m##_wrapper(Component_t* pComp) \
		{ \
		  ((COMPONENT_TYPE*)pComp)->i.m = (void *)pComp->pMsgQueue->pRead->pMsgId; \
		  printf("Transition: %s | %s | %s \n", #s, #i, #m); \
		  _##s##_##i##_##m((COMPONENT_TYPE*)pComp); \
		  ((COMPONENT_TYPE*)pComp)->i.m = NULL; \
		} \
		static void _##s##_##i##_##m(COMPONENT_TYPE* pComp)

// -------------------------------------------------------------------------------------------------

#define EVALUATION_FUNCTION(n) \
		static bool n(COMPONENT_TYPE* pComp); \
		static bool n##_wrapper(Component_t* pComp) \
		{ \
			return n((COMPONENT_TYPE*)pComp); \
		} \
		static bool n(COMPONENT_TYPE* pComp)

// -------------------------------------------------------------------------------------------------

#define MESSAGE_CHECK(i,m) \
		assert( pComp->i.m == NULL )

#define PREPARE_MESSAGE(i,m) \
		pComp->i.m = PrepareMessage(pComp->pMsgQueue, pComp->i.p##m##_id, MESSAGE_SIZE(pComp->i.m))

#define SEND_MESSAGE(i,m) \
		SendMessage(pComp->pMsgQueue, (Interface_t*)&pComp->i); \
		pComp->i.m = NULL;

// -------------------------------------------------------------------------------------------------

#define SET_TRANS_1 _trans_1(pComp)
#define SET_TRANS_2 SET_TRANS_1; _trans_2(pComp)
#define SET_TRANS_3 SET_TRANS_2; _trans_3(pComp)
#define SET_TRANS_4 SET_TRANS_3; _trans_4(pComp)
#define SET_TRANS_5 SET_TRANS_4; _trans_5(pComp)
#define SET_TRANS_6 SET_TRANS_5; _trans_6(pComp)
#define SET_TRANS_7 SET_TRANS_6; _trans_7(pComp)
#define SET_TRANS_8 SET_TRANS_7; _trans_8(pComp)
#define SET_TRANS_9 SET_TRANS_8; _trans_9(pComp)
#define SET_TRANS_10 SET_TRANS_9; _trans_10(pComp)
#define SET_TRANS_11 SET_TRANS_10; _trans_11(pComp)
#define SET_TRANS_12 SET_TRANS_11; _trans_12(pComp)
#define SET_TRANS_13 SET_TRANS_12; _trans_13(pComp)
#define SET_TRANS_14 SET_TRANS_13; _trans_14(pComp)
#define SET_TRANS_15 SET_TRANS_14; _trans_15(pComp)
#define SET_TRANS_16 SET_TRANS_15; _trans_16(pComp)
#define SET_TRANS_17 SET_TRANS_16; _trans_17(pComp)
#define SET_TRANS_18 SET_TRANS_17; _trans_18(pComp)
#define SET_TRANS_19 SET_TRANS_18; _trans_19(pComp)
#define SET_TRANS_20 SET_TRANS_19; _trans_20(pComp)
#define SET_TRANS_21 SET_TRANS_20; _trans_21(pComp)
#define SET_TRANS_22 SET_TRANS_21; _trans_22(pComp)
#define SET_TRANS_23 SET_TRANS_22; _trans_23(pComp)
#define SET_TRANS_24 SET_TRANS_23; _trans_24(pComp)
#define SET_TRANS_25 SET_TRANS_24; _trans_25(pComp)
#define SET_TRANS_26 SET_TRANS_25; _trans_26(pComp)
#define SET_TRANS_27 SET_TRANS_26; _trans_27(pComp)
#define SET_TRANS_28 SET_TRANS_27; _trans_28(pComp)
#define SET_TRANS_29 SET_TRANS_28; _trans_29(pComp)
#define SET_TRANS_30 SET_TRANS_29; _trans_30(pComp)
// Extend this when more than 30 transitions.

#define SET_TRANSITIONS(x) \
		SET_TRANS_##x

#endif
