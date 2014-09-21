// -------------------------------------------------------------------------------------------------
// Control.c
// Control interface for demo device
// -------------------------------------------------------------------------------------------------

#include "Control.h"

// -------------------------------------------------------------------------------------------------
// Control_init
// Interface initialization.
// -------------------------------------------------------------------------------------------------
void Control_init( Control_t* pInterface)
{
	SET_MESSAGE_ID(StartInd);
	SET_MESSAGE_ID(PauseInd);
	SET_MESSAGE_ID(RestartInd);
	SET_MESSAGE_ID(QuitInd);
}



