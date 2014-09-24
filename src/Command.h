// -------------------------------------------------------------------------------------------------
// Command.h
// Command interface for the demo device.
// -------------------------------------------------------------------------------------------------

#ifndef COMMAND_H
#define COMMAND_H

#include "Types.h"
#include "Interface.h"

// -------------------------------------------------------------------------------------------------
// Command interface definition
// -------------------------------------------------------------------------------------------------
typedef struct
{
	INTERFACE_BASETYPE_FIELDS;
	MESSAGE_DEF(SpeedInd,
			int acceleration;
	);
	MESSAGE_DEF(SteerInd,
			int direction;
	);
} Command_t;


void Command_init(Command_t* pInterface);

#endif
