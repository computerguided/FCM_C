// -------------------------------------------------------------------------------------------------
// CommandHandler.h
//
// -------------------------------------------------------------------------------------------------

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "Types.h"

#include "Command.h"
#include "Component.h"
#include "Control.h"
#include "Display.h"


#undef COMPONENT_TYPE
#define COMPONENT_TYPE CommandHandler_t

typedef struct
{
	COMPONENT_BASETYPE_FIELDS;

	// -- Interfaces --
	Control_t Control;
	Command_t Command;

	// -- States --
	State_t Idle;

	// -- State variables --

} COMPONENT_TYPE;

void CommandHandler_init(COMPONENT_TYPE* pComp, MessageQueue_t* pMsgQueue);

void Keystroke(void *pComponent, int messageIndex);

#endif
