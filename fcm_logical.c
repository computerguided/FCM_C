// -------------------------------------------------------------------------------------------------
// fcm_ogical.c
// -------------------------------------------------------------------------------------------------

#include "fcm_logical.h"

void logical_init( fcm_logical_t* p_interface)
{
	FCM_SET_MESSAGE_ID(yes);
	FCM_SET_MESSAGE_ID(no);
	p_interface->p_name = "Logical";
}
