// -------------------------------------------------------------------------------------------------
// Controller.c
// Controller component for the demo device.
// -------------------------------------------------------------------------------------------------

#include "Controller.h"


#undef COMPONENT_TYPE
#define COMPONENT_TYPE Controller_t

// -------------------------------------------------------------------------------------------------
// Transition functions
// Macro: TRANSITION_FUNCTION(<state>,<interface>,<message>)
// -------------------------------------------------------------------------------------------------


//Idle -> Running:StartInd
TRANSITION_FUNCTION(Running,Control,StartInd){}

//Running -> Collision:TimeoutInd
TRANSITION_FUNCTION(Running,Timer,TimeoutInd){}

//GameOver -> Running:RestartInd
TRANSITION_FUNCTION(GameOver,Control,RestartInd){}

//Running ->Paused:PauseInd
TRANSITION_FUNCTION(Running,Control,PauseInd){}

//Paused -> Running:PauseInd
TRANSITION_FUNCTION(Paused,Control,PauseInd){}

//Collision -> Running:No
TRANSITION_FUNCTION(Collision,Logical,No){}

//Collision -> GameOver:Yes
TRANSITION_FUNCTION(Collision,Logical,Yes){}

//GameOver -> Idle:QuitInd
TRANSITION_FUNCTION(GameOver,Control,QuitInd){}

//Running -> Idle:QuitInd
TRANSITION_FUNCTION(Running,Control,QuitInd){}

//Paused -> Idle:QuitInd


TRANSITION_FUNCTION(Idle,Command,SpeedInd){}







TRANSITION_FUNCTION(Paused,Control,QuitInd){}









// -------------------------------------------------------------------------------------------------
// Controller_init
// -------------------------------------------------------------------------------------------------
void Controller_init( Controller_t* pComp )
{
	SET_STATE(Idle,NULL);
	SET_STATE(Running,NULL);
	SET_STATE(Paused,NULL);
	SET_STATE(Collision,NULL);
	SET_STATE(GameOver,NULL);

	SET_CURRENT_STATE(Idle);

	// Set transitions
	TRANSITION(Idle,Control,StartInd,Running);
	TRANSITION(Running,Control,StartInd,Running);



}

