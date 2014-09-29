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
	SET_MSG_QUEUE(pMsgQueue);

	pComp->currentTime = 1;

}

// -------------------------------------------------------------------------------------------------
void _TimerTick(void* pComponent, int resourceIndex)
{
	TimerHandler_t* pComp = pComponent;
	while( pComp->pNextTimeout->timeout == pComp->currentTime )
	{
		PREPARE_MESSAGE(Timer,TimeoutInd);

		// Expand SEND_MESSAGE to determine remote interface explicitly.
		pComp->Timer.TimeoutInd->timerId = pComp->pNextTimeout->timerId;
		pComp->pMsgQueue->pWrite->pInterface = (Interface_t*)&pComp->pNextTimeout->pComponent->Timer; // Differs from macro.
		pComp->pMsgQueue->pWrite->systemTime = GetSystemTime();
		pComp->pMsgQueue->pWrite = (void*)((address_t)&pComp->pMsgQueue->pWrite->pMsgId
		+ pComp->pMsgQueue->pWrite->msgSize);
		pComp->Timer.TimeoutInd = NULL;

		// The timeout entry is 'emptied'.
		pComp->pNextTimeout->timerId = 0;

		// The pointer is shifted one index and wraps around when at the end of the array.
		if( ++pComp->nextTimeoutIndex == MAX_TIMERS) pComp->nextTimeoutIndex = 0;
		pComp->pNextTimeout = pComp->timeout[pComp->nextTimeoutIndex];
	}

	pComp->currentTime++;
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
	static uint32_t _timerId = 1;

	// Get the pointer to the connected Timer Handler.
	TimerHandler_t* pTimerHandler = ((Interface_t*)pComp->Timer.pRemoteInterface)->pComponent;

	TimeoutEntry_t* newEntry;


<hier>







	// Can only add when slot empty.
	if( !pTimerHandler->pNextNewTimeout->timerId ) return 0;

	pTimerHandler->pNextTimeout = pTimerHandler->currentTime+interval;




	_timerId++;




	return _timerId;
}

void CancelTimer(Component_t *pComp, uint32_t timerid)
{

}
