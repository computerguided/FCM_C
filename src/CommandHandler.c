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
