// -------------------------------------------------------------------------------------------------
// Component.h
// -------------------------------------------------------------------------------------------------

#ifndef COMPONENT_H
#define COMPONENT_H

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
	Logical_t logical;
	Timer_t timer;
} Component_t;

#define COMPONENT_BASETYPE_FIELDS \
		SttElement_t* pCurrentState; \
		SttElement_t* pTransitions; \
		MessageQueue_t* pMsgQueue; \
		Logical_t logical; \
		Timer_t timer;

typedef void (*TransitionFunction_t)(Component_t*);
typedef bool (*EvaluationFunction_t)(Component_t*);

typedef struct
{
	char* pName;
	EvaluationFunction_t pEvaluation;
} State_t;

#define SET_STATE(s,e) \
		s.pName = #s; \
		s.pEvaluation = e

// -------------------------------------------------------------------------------------------------
// Macros to be used for/in transition function.
// -------------------------------------------------------------------------------------------------

#define TRANSITION_FUNCTION(s,i,m) \
		void s##i##m(COMPONENT_TYPE* pComp); \
		void s##i##m##_wrapper(Component_t* pComp) \
		{ \
			s##i##.m = &pComp->pMsgQueue->pRead->id; \
			s##i##m((COMPONENT_TYPE *)pComp); \
			s##i##.m = NULL; \
		} \
		void s##i##m(COMPONENT_TYPE *pComp)

#define PREPARE_MESSAGE(i,m) \
		pComp->i.m = PrepareMessage(pComp->pMsgQueue, pComp->i.m##_id, MESSAGE_SIZE(pComp->i.m))

#define SEND_MESSAGE(i,m) \
		pComp->pMsgQueue->pWrite->interface = pComp->i.pRemoteInterface; \
		pComp->pMsgQueue->pWrite->systemTime = GetSystemTime(); \
		pComp->pMsgQueue->pWrite = &pComp->pMsgQueue->pWrite->pMsgId + pComp->pMsgQueue->pWrite->msgSize; \
		pComp->i.m = NULL;

#endif
