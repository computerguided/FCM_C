// -------------------------------------------------------------------------------------------------
// Logical.c
// -------------------------------------------------------------------------------------------------

#include "Logical.h"

void Logical_init( Logical_t* pInterface)
{
	SET_MESSAGE_ID(Yes);
	SET_MESSAGE_ID(No);
	pInterface->pName = "Logical";
}
