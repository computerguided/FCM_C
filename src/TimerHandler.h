// -------------------------------------------------------------------------------------------------
// TimerHandler.h
//
// -------------------------------------------------------------------------------------------------

#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include "Types.h"
#include "Component.h"

#undef COMPONENT_TYPE
#define COMPONENT_TYPE TimerHandler_t

#define MAX_TIMERS 10 // Adjust when more timers are required.

typedef struct
{
  uint32_t timerId;
  Component_t* pComponent;
  uint32_t timeout;
} TimeoutEntry_t;

typedef struct
{
	COMPONENT_BASETYPE_FIELDS;
	TimeoutEntry_t timeout[MAX_TIMERS];
	uint32_t currentTime;
} COMPONENT_TYPE;

void TimerHandler_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue);

uint32_t InformIn(Component_t *pComp, uint32_t interval );
void CancelTimer(Component_t *pComp, uint32_t timerid);

#endif
