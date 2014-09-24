// -------------------------------------------------------------------------------------------------
// Controller.c
// Controller component for the demo device.
// -------------------------------------------------------------------------------------------------

#include "Controller.h"

// -------------------------------------------------------------------------------------------------
// Transition functions
// Macro: TRANSITION_FUNCTION(<state>,<interface>,<message>)
// -------------------------------------------------------------------------------------------------

TRANSITION_FUNCTION(1,	Idle,		Control,	StartInd, 	Running)
{
}
TRANSITION_FUNCTION(2,	Running,	Timer,		TimeoutInd,	Collision)
{
}
TRANSITION_FUNCTION(3,	Running,	Control,	QuitInd,	Idle)
{
}
TRANSITION_FUNCTION(4,	Running,	Control,	PauseInd,	Paused)
{
}
TRANSITION_FUNCTION(5,	Running,	Command,	SpeedInd,	Running)
{
}
TRANSITION_FUNCTION(6,	Paused,		Control,	PauseInd,	Running)
{
}
TRANSITION_FUNCTION(7,	Paused,		Control,	QuitInd,	Idle)
{
}
TRANSITION_FUNCTION(8,	Collision,	Logical,	No,			Running)
{
}
TRANSITION_FUNCTION(9,	Collision,	Logical,	Yes,		GameOver)
{}
TRANSITION_FUNCTION(10,	GameOver,	Control,	QuitInd,	Idle)
{}
TRANSITION_FUNCTION(11,	GameOver,	Control,	RestartInd,	Running)
{}


#undef NUM_TRANSITIONS
#define NUM_TRANSITIONS 11

// -------------------------------------------------------------------------------------------------
// Evaluation functions
// -------------------------------------------------------------------------------------------------
EVALUATION_FUNCTION(Collision)
{
	return true;
}

// -------------------------------------------------------------------------------------------------
void Controller_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue)
{
	// Initialize interfaces.
	SET_INTERFACE(Display);
	SET_INTERFACE(Control);
	SET_INTERFACE(Command);

	// Set states.
	SET_STATE(Idle);
	SET_STATE(Running);
	SET_STATE(Paused);
	SET_STATE(GameOver);

	// Set choicepoints.
	SET_CHOICEPOINT(Collision);

	// Initialize state transition table.
	INIT_STT(NUM_TRANSITIONS,Idle);

	SET_MSG_QUEUE(pMsgQueue);

	// -- Initialize state variables --
	pComp->speed = 5;
	pComp->position = 40;
}
