// -------------------------------------------------------------------------------------------------
// Command.c
// Command interface for the demo device.
// -------------------------------------------------------------------------------------------------

#include "Command.h"

void Command_init( Command_t* pInterface)
{
	SET_MESSAGE_ID(SpeedInd);
	SET_MESSAGE_ID(SteerInd);
}
