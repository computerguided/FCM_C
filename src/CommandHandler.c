// -------------------------------------------------------------------------------------------------
// CommandHandler.c
// Command Handler component for the demo device.
// -------------------------------------------------------------------------------------------------

#include "CommandHandler.h"

// -------------------------------------------------------------------------------------------------
// Transition functions
// Macro: TRANSITION_FUNCTION(<state>,<interface>,<message>)
// -------------------------------------------------------------------------------------------------

TRANSITION_FUNCTION(1, Idle, Control, StartInd, Idle)
{}

#undef NUM_TRANSITIONS
#define NUM_TRANSITIONS 1


// -------------------------------------------------------------------------------------------------
// CommandHandler_init
// -------------------------------------------------------------------------------------------------
void CommandHandler_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue)
{
	// -- Initialize interfaces --
	SET_INTERFACE(Timer);
	SET_INTERFACE(Logical);
	SET_INTERFACE(Control);
	SET_INTERFACE(Command);

	// -- Set state ids and evaluation functions --
	SET_STATE(Idle);

	// Initialize state transition table.
	INIT_STT(NUM_TRANSITIONS,Idle);

	SET_MSG_QUEUE(pMsgQueue);
}

// -------------------------------------------------------------------------------------------------
// Keystroke
// -------------------------------------------------------------------------------------------------
// Messaging function
// -------------------------------------------------------------------------------------------------
void Keystroke(void *pComponent, int resourceIndex)
{
	COMPONENT_TYPE *pComp = pComponent;

	if( resourceIndex == 0 )
	{

		PREPARE_MESSAGE(Control,StartInd);

		pComp->pMsgQueue->pWrite->pInterface = pComp->Control.pRemoteInterface; \
		pComp->pMsgQueue->pWrite->systemTime = GetSystemTime(); \
		pComp->pMsgQueue->pWrite = (void *)((address_t)&pComp->pMsgQueue->pWrite->pMsgId + pComp->pMsgQueue->pWrite->msgSize); \
		pComp->Control.StartInd = NULL;
	}
	else if( resourceIndex == 1 )
	{
		PREPARE_MESSAGE(Control,QuitInd);

		pComp->pMsgQueue->pWrite->pInterface = pComp->Control.pRemoteInterface; \
		pComp->pMsgQueue->pWrite->systemTime = GetSystemTime(); \
		pComp->pMsgQueue->pWrite = (void *)((address_t)&pComp->pMsgQueue->pWrite->pMsgId + pComp->pMsgQueue->pWrite->msgSize); \
		pComp->Control.QuitInd = NULL;

	}
	else if( resourceIndex == 2 )
	{
		PREPARE_MESSAGE(Control,QuitInd);

		pComp->pMsgQueue->pWrite->pInterface = pComp->Control.pRemoteInterface; \
		pComp->pMsgQueue->pWrite->systemTime = GetSystemTime(); \
		pComp->pMsgQueue->pWrite = (void *)((address_t)&pComp->pMsgQueue->pWrite->pMsgId + pComp->pMsgQueue->pWrite->msgSize); \
		pComp->Control.QuitInd = NULL;

	}



}
