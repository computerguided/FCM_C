// -------------------------------------------------------------------------------------------------
// Component.h
// -------------------------------------------------------------------------------------------------

#ifndef COMPONENT_H
#define COMPONENT_H

#include "Types.h"
#include "StateTransitionTable.h"
#include "Logical.h"
#include "MessageQueue.h"

#define SET_STATE(s,e) \
s.pName = #s; \
s.pEvaluation = e

typedef struct
{
	SttElement_t* pCurrentState;
	SttElement_t* pTransitions;
	MessageQueue_t* pMsgQueue;
	//Logical_t logical;
	//Timer_t timer;
} Component_t;

#define COMPONENT_BASETYPE_FIELDS \
SttElement_t* pCurrentState; \
SttElement_t* pTransitions; \
MessageQueue_t* pMsgQueue

#define TRANSITION_FUNCTION(s,i,m) \
void s##i##m(COMPONENT_TYPE *comp); \
void s##i##m##_wrapper(Component_t *comp) \
{ \
	s##i##.m = &comp->pMsgQueue->pRead->id; \
	s##i##m((COMPONENT_TYPE *)comp); \
	s##i##.m = NULL; \
} \
void s##i##m(COMPONENT_TYPE *comp)

typedef void (*TransitionFunction_t)(Component_t*);

#endif
