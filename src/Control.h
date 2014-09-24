// -------------------------------------------------------------------------------------------------
// Control.h
// Demo device
// Control interface definition.
// -------------------------------------------------------------------------------------------------

#ifndef CONTROL_H
#define CONTROL_H

#include "Types.h"
#include "Interface.h"

typedef struct
{
	INTERFACE_BASETYPE_FIELDS;
	MESSAGE_DEF(StartInd);
	MESSAGE_DEF(PauseInd);
	MESSAGE_DEF(RestartInd);
	MESSAGE_DEF(QuitInd);
} Control_t;

void Control_init( Control_t* pInterface);

#endif
