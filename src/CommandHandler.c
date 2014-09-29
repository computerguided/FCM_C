// -------------------------------------------------------------------------------------------------
// CommandHandler.c
// Command Handler component for the demo device.
// -------------------------------------------------------------------------------------------------

#include "CommandHandler.h"

//typedef struct
//{
//	COMPONENT_BASETYPE_FIELDS;
//	STT(1);
//
//	// -- Interfaces --
//	Control_t Control;
//	Command_t Command;
//
//	// -- States --
//	State_t Idle;
//
//	// -- State variables --
//
//} CommandHandler_t;


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
