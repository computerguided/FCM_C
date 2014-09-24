// -------------------------------------------------------------------------------------------------
// Display.h
// Display interface for the demo device.
// -------------------------------------------------------------------------------------------------

#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"
#include "Interface.h"


typedef struct
{
	INTERFACE_BASETYPE_FIELDS;
	MESSAGE_DEF(DisplayInd,
			char* pLine;
	);
} Display_t;

void Display_init( Display_t* pInterface);


#endif
