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
