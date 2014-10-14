// -------------------------------------------------------------------------------------------------
// CommandHandler.c
// Command Handler component for the demo device.
// -------------------------------------------------------------------------------------------------

#include "CommandHandler.h"

// -------------------------------------------------------------------------------------------------
// Transition functions
// Macro: TRANSITION_FUNCTION(<state>,<interface>,<message>)
// -------------------------------------------------------------------------------------------------

#undef NUM_TRANSITIONS
#define NUM_TRANSITIONS 1

TRANSITION_FUNCTION(1, Idle, Control, StartInd, Idle)
{}




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
void Keystroke(void *pComponent, int testMessageIndex)
{
	COMPONENT_TYPE *pComp = pComponent;

//	MESSAGE_DEF(StartInd);
//	MESSAGE_DEF(PauseInd);
//	MESSAGE_DEF(RestartInd);
//	MESSAGE_DEF(QuitInd);


	switch (testMessageIndex )
	{
	case 1:
		PREPARE_MESSAGE(Control,StartInd);
		SEND_MESSAGE(Control,StartInd);
		break;

	case 2:
		PREPARE_MESSAGE(Control,PauseInd);
		SEND_MESSAGE(Control,PauseInd);
		break;

	case 3:
		PREPARE_MESSAGE(Control,RestartInd);
		SEND_MESSAGE(Control,RestartInd);
		break;

	case 4:
		PREPARE_MESSAGE(Control,QuitInd);
		SEND_MESSAGE(Control,QuitInd);
		break;

	case 5:
		PREPARE_MESSAGE(Command,SpeedInd);
		pComp->Command.SpeedInd->acceleration = 99;
		SEND_MESSAGE(Control,QuitInd);
		break;

	}

}
