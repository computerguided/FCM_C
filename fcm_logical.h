// -------------------------------------------------------------------------------------------------
// fcm_logical.h
// -------------------------------------------------------------------------------------------------

#ifndef FCM_LOGICAL_H
#define FCM_LOGICAL_H

#include "fcm_interface.h"
#include "fcm_types.h"


typedef struct
{
	FCM_INTERFACE_BASETYPE_FIELDS;
	FCM_MESSAGE_DEF(yes);
	FCM_MESSAGE_DEF(no);
} fcm_logical_t;

// -------------------------------------------------------------------------------------------------
// Logical_init
// -------------------------------------------------------------------------------------------------
// pInterface: pointer to the Logical interface.
// -------------------------------------------------------------------------------------------------
void logical_init(fcm_logical_t* p_interface);



#endif
