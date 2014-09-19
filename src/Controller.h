// -------------------------------------------------------------------------------------------------
// Controller.h
//
// -------------------------------------------------------------------------------------------------

#ifndef CONTROLLER_H
#define CONTROLLER_H


#include "Types.h"

#include "Component.h"
#include "Control.h"
#include "Display.h"
#include "Command.h"

typedef struct
{
	COMPONENT_BASETYPE_FIELDS;
	STT(11*TRANSITION_SIZE); // STT(<number of transitions>*TRANSITION_SIZE);

	// -- Interfaces --
	Display_t Display;
	Control_t Control;
	Command_t Command;

	// -- States --
	State_t Idle;
	State_t Running;
	State_t Paused;
	State_t Collision;
	State_t GameOver;

	// -- State variables --
	int position;
	int speed;

} Controller_t;


#endif
