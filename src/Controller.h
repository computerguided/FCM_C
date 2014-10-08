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

#undef COMPONENT_TYPE
#define COMPONENT_TYPE Controller_t

typedef struct
{
	COMPONENT_BASETYPE_FIELDS;

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

} COMPONENT_TYPE;

void Controller_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue);

#endif
