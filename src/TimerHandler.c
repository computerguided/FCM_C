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

	// Initialize state variables.

	int i;
	for(i=0; i<MAX_TIMERS; i++)
	{
		pComp->timeout[i].timeout = 0;
		pComp->timeout[i].timerId = 0;
	}

	pComp->pNextTimeout = pComp->timeout;
	pComp->nextTimeoutIndex = 0;
	pComp->endIndex = 0;
	pComp->currentTime = 1;
}

// -------------------------------------------------------------------------------------------------
// _TimerTick
// -------------------------------------------------------------------------------------------------
// Messaging function called in the timer ISR. It checks whether there is a timeout and if so
// sends a TimeoutInd to the component that requested the timer.
// The function also increases the currentTime and as such is called every timer tick.
// -------------------------------------------------------------------------------------------------
// pComponent : pointer to the component that requested the timer.
// resourceIndex : required to satisfy the messaging function prototype, but not used here.
// -------------------------------------------------------------------------------------------------
void _TimerTick(void* pComponent, int resourceIndex)
{
	TimerHandler_t* pComp = pComponent;
	while( pComp->pNextTimeout->timeout == pComp->currentTime ) // This is why we need to start currentTime at 1.
	{
		if( pComp->pNextTimeout->timerId != 0 ) // Skip cancelled timer.
		{
			// -- Timeout! --

			PREPARE_MESSAGE(Timer,TimeoutInd);


			pComp->Timer.TimeoutInd->timerId = pComp->pNextTimeout->timerId;

			// Expand SEND_MESSAGE to determine remote interface explicitly.

			pComp->pMsgQueue->pWrite->systemTime = GetSystemTime();
			pComp->pMsgQueue->pWrite->pInterface = (Interface_t*)&pComp->pNextTimeout->pComponent->Timer; // Differs from macro.
			ShiftWritePointer(pComp->pMsgQueue);

			pComp->Timer.TimeoutInd = NULL;

			// Delete the timer.
			pComp->pNextTimeout->timerId = 0;
			pComp->pNextTimeout->timeout = 0;
		}

		// When at this point there is still a value !=0 stored in the timeout, then it
		// was a

		// The pointer is shifted one index and wraps around when at the end of the array.
		if( ++pComp->nextTimeoutIndex == MAX_TIMERS) pComp->nextTimeoutIndex = 0;
		pComp->pNextTimeout = &pComp->timeout[pComp->nextTimeoutIndex];
	}

	pComp->currentTime++;
}


// ---- Timer functions ----

// -------------------------------------------------------------------------------------------------
// InformIn
// -------------------------------------------------------------------------------------------------
// Set a timer. Returns the timer id, or 0 when failed.
// Failure can occur when no more timers can be add or interval below MIN_INTERVAL.
// -------------------------------------------------------------------------------------------------
// pComp : pointer to the component that requested the timer.
// interval : number of milliseconds after the currentTime at which the timer needs to expire.
// -------------------------------------------------------------------------------------------------
uint32_t InformIn(Component_t *pComp, uint32_t interval )
{
	static uint32_t _timerId = 0;

	// Get the pointer to the Timer Handler connected to the supplied component.
	TimerHandler_t* pTimerHandler = ((Interface_t*)pComp->Timer.pRemoteInterface)->pComponent;

	// Can only add when empty slot available.
	if( pTimerHandler->timeout[pTimerHandler->endIndex].timerId != 0 ) return 0;

	// 'Create' new timer id.
	_timerId++;

	// Do not allow a too small interval to prevent problem when this function is preempted (will happen).
	if( interval < MIN_INTERVAL ) return 0;

	// Determine the timeout value.
	uint64_t timeout = pTimerHandler->currentTime+interval;

	/*
	 * Note: all the code here can be interrupted by the timer ISR _TimerTick().
	 * This needs to be taken into account.
	 * The ISR will shift the pTimerHandler->pNextTimeout and set the timerId and timeout to 0.
	 */

	// Find the location where to insert the new timeout.
	int elementIndex = pTimerHandler->nextTimeoutIndex; // Begin of the queue.
	while( elementIndex != pTimerHandler->endIndex)
	{
		// Compare timeouts, and break if insert location found.
		if( pTimerHandler->timeout[elementIndex].timeout > timeout ) break;

		// Shift down.
		if( ++elementIndex == MAX_TIMERS) elementIndex = 0;
	}

	// Shift all elements down to make room for next entry.
	int shiftIndex = pTimerHandler->endIndex;
	int aboveIndex;
	while( shiftIndex != elementIndex )
	{
		aboveIndex = (shiftIndex==0)?MAX_TIMERS-1:shiftIndex-1;

		// Copy
		pTimerHandler->timeout[shiftIndex] = pTimerHandler->timeout[aboveIndex];

		// Shift up.
		if( --shiftIndex == -1 ) shiftIndex = MAX_TIMERS-1;
	}

	// Shift end.
	if( ++pTimerHandler->endIndex == MAX_TIMERS) pTimerHandler->endIndex = 0;

	// Place new timeout.
	pTimerHandler->timeout[elementIndex].pComponent = pComp;
	pTimerHandler->timeout[elementIndex].timerId = _timerId;
	pTimerHandler->timeout[elementIndex].timeout = timeout;

	return _timerId;
}


// -------------------------------------------------------------------------------------------------
// DeleteTimoutInd
// -------------------------------------------------------------------------------------------------
// Shift through the indicated message queue and find the right interface and
// -------------------------------------------------------------------------------------------------
bool DeleteTimeoutInd(MessageQueue_t* pMsgQueue, Timer_t* pInterface, uint32_t timerId  )
{
	Message_t* pMsg = pMsgQueue->pRead;

	while( pMsg != pMsgQueue->pWrite )
	{
		if( pMsg->pInterface == (Interface_t *)pInterface &&
			pInterface->TimeoutInd->pMsgId == pMsg->pMsgId &&
			pInterface->TimeoutInd->timerId == timerId )
		{
			// Message found, 'delete' it.
			pMsg->pInterface = NULL;
			return true;
		}

		// Shift to next message.
		pMsg = (void *)((address_t)&pMsg->pMsgId+pMsg->msgSize);
		if( pMsg == pMsgQueue->pWrapAround ) pMsg = pMsgQueue->pMessage;
	}
	return false;
}

// -------------------------------------------------------------------------------------------------
// CancelTimer
// -------------------------------------------------------------------------------------------------
// Cancel a timer. If the timeout already occurred, the Timeout message is deleted in the
// message queue.
// -------------------------------------------------------------------------------------------------
// pComp : pointer to the component that wants to cancel the timer.
// timerId : the id of the timer which is to be cancelled. Was given as return value of InformIn().
// -------------------------------------------------------------------------------------------------
bool CancelTimer(Component_t* pComp, uint32_t timerId)
{
	// Get the pointer to the Timer Handler connected to the supplied component.
	TimerHandler_t* pTimerHandler = ((Interface_t*)pComp->Timer.pRemoteInterface)->pComponent;

	// Find the indicated timer id.
	int elementIndex = pTimerHandler->nextTimeoutIndex;

	while( elementIndex != pTimerHandler->endIndex )
	{
		if( pTimerHandler->timeout[elementIndex].timerId == timerId ) break;
		if( ++elementIndex == MAX_TIMERS) elementIndex = 0;
	}

	if( elementIndex == pTimerHandler->endIndex ) return false;

	pTimerHandler->timeout[elementIndex].timerId = 0;

	if( pTimerHandler->timeout[elementIndex].timeout != 0 ) return true;

	// Try to delete the timeout indication from the asynchronous queue. If not found,
	// try deleting it from the message queue of the component (probably main message queue).
	bool result = DeleteTimeoutInd(pTimerHandler->pMsgQueue, &pComp->Timer, timerId);
	if( !result ) result = DeleteTimeoutInd(pComp->pMsgQueue, &pComp->Timer, timerId);
	return result;
}


