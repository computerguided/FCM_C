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

typedef struct
{
	SttElement_t* pCurrentState;
	SttElement_t* pTransitions;
	MessageQueue_t* pMsgQueue;
	Logical_t Logical;
	Timer_t Timer;
} Component_t;

#define COMPONENT_BASETYPE_FIELDS \
		SttElement_t* pCurrentState; \
		SttElement_t* pTransitions; \
		MessageQueue_t* pMsgQueue; \
		Logical_t Logical; \
		Timer_t Timer;

typedef void (*TransitionFunction_t)(Component_t*);
typedef bool (*EvaluationFunction_t)(Component_t*);

typedef struct
{
	char* pName;
	EvaluationFunction_t pEvaluation;
} State_t;


// -------------------------------------------------------------------------------------------------
// Macro in the component initialization.
// -------------------------------------------------------------------------------------------------

#define SET_STATE(s,e) \
		pComp->s.pName = #s; \
		pComp->s.pEvaluation = e

#define SET_CURRENT_STATE(s) \
		pComp->pCurrentState = pComp->s


// -------------------------------------------------------------------------------------------------
// Macros to be used for/in transition function.
// -------------------------------------------------------------------------------------------------

#define TRANSITION_FUNCTION(s,i,m) \
		void s##_##i##_##m(COMPONENT_TYPE* pComp); \
		void s##_##i##_##m##_wrapper(Component_t* pComp) \
		{ \
		  ((COMPONENT_TYPE*)pComp)->i.m = &pComp->pMsgQueue->pRead->pMsgId; \
		  s##_##i##_##m((COMPONENT_TYPE*)pComp); \
		  ((COMPONENT_TYPE*)pComp)->i.m = NULL; \
		} \
		void s##_##i##_##m(COMPONENT_TYPE *pComp)

#define TRANSITION(s,i,m,n) \
	Transition( \
			pComp->pTransitions, \
			&pComp->s, \
			&pComp->i, \
			pComp->i.p##m##_id, \
			s##_##i##_##m##_wrapper, \
			&pComp->n )

//Transition(
//		pComp->pTransitions,
//		&pComp->Idle,
//		&pComp->Control,
//		pComp->Control.pRestartInd_id,
//		Idle_Control_StartInd_wrapper,
//		&pComp->Running );

#define MESSAGE_CHECK(i,m) \
		assert( pComp->i.m == NULL )

#define PREPARE_MESSAGE(i,m) \
		pComp->i.m = PrepareMessage(pComp->pMsgQueue, pComp->i.m##_id, MESSAGE_SIZE(pComp->i.m))

#define SEND_MESSAGE(i,m) \
		pComp->pMsgQueue->pWrite->interface = pComp->i.pRemoteInterface; \
		pComp->pMsgQueue->pWrite->systemTime = GetSystemTime(); \
		pComp->pMsgQueue->pWrite = &pComp->pMsgQueue->pWrite->pMsgId + pComp->pMsgQueue->pWrite->msgSize; \
		pComp->i.m = NULL;

#endif
