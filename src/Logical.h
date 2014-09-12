// -------------------------------------------------------------------------------------------------
// Logical.h
// -------------------------------------------------------------------------------------------------

#ifndef LOGICAL_H
#define LOGICAL_H

#include "Types.h"
#include "Interface.h"


typedef struct
{
	INTERFACE_BASETYPE_FIELDS;
	MESSAGE_DEF(Yes);
	MESSAGE_DEF(No);
} Logical_t;




#endif
