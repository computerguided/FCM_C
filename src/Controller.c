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

// -------------------------------------------------------------------------------------------------
EVALUATION_FUNCTION(Collision)
{
	return true;
}

// -------------------------------------------------------------------------------------------------
// Controller_init
// -------------------------------------------------------------------------------------------------
void Controller_init(Controller_t* pComp)
{
	// -- Set state ids and evaluation functions --
	SET_STATE(Idle);
	SET_STATE(Running);
	SET_STATE(Paused);
	SET_CHOICEPOINT(Collision);
	SET_STATE(GameOver);

	// -- Initialize state transition table --
	SET_STT;
	SET_TRANS_11; //SET_TRANS_<number of transitions>
	SET_FIRST_STATE(Idle);

	// -- Initialize state variables --
	pComp->speed = 5;
	pComp->position = 40;
}
