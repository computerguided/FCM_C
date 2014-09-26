// -------------------------------------------------------------------------------------------------
// TimerHandler.c
// Timer Handler component.
// -------------------------------------------------------------------------------------------------

#include "TimerHandler.h"

// -------------------------------------------------------------------------------------------------
// TimerHandler_init
// -------------------------------------------------------------------------------------------------
void TimerHandler_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue)
{
	// -- Initialize interfaces --
	SET_INTERFACE(Timer);
	SET_INTERFACE(Logical);
	SET_MSG_QUEUE(pMsgQueue);

	pComp->currentTime = 0;
}


// -------------------------------------------------------------------------------------------------
void TimerTick()
{

}


// ---- Timer functions ----

// -------------------------------------------------------------------------------------------------
// InformIn
// -------------------------------------------------------------------------------------------------
// Set a timer.
// -------------------------------------------------------------------------------------------------
// pComp : pointer to the component that requested the timer.
// interval : number of milliseconds after NOW at which the timer needs to expire.
// -------------------------------------------------------------------------------------------------
uint32_t InformIn(Component_t *pComp, uint32_t interval )
{
	static uint32_t timerId = 0;

	timerId++;

	uint32_t timeout = 	GetSystemTime() + interval;








	return timerId;
}

void CancelTimer(Component_t *pComp, uint32_t timerid)
{

}
