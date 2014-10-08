// -------------------------------------------------------------------------------------------------
// TimerHandler.h
//
// -------------------------------------------------------------------------------------------------

#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include "Types.h"
#include "Component.h"

typedef struct
{
	uint32_t timerId;
	Component_t* pComponent;
	uint64_t timeout;
} TimeoutEntry_t;

#undef COMPONENT_TYPE
#define COMPONENT_TYPE TimerHandler_t

#define MAX_TIMERS 10 // Adjust when more timers are required.

typedef struct
{
	MessageQueue_t* pMsgQueue;
	Timer_t Timer;
	TimeoutEntry_t timeout[MAX_TIMERS];
	TimeoutEntry_t* pNextTimeout;
	int nextTimeoutIndex;
	int endIndex;
	uint64_t currentTime;
} COMPONENT_TYPE;

void TimerHandler_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue);

#define MIN_INTERVAL 5 // Minimum interval to handle preempting while adding timeout.
uint32_t InformIn(Component_t *pComp, uint32_t interval );
bool CancelTimer(Component_t *pComp, uint32_t timerid);

#endif
