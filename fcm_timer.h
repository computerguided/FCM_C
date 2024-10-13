// -------------------------------------------------------------------------------------------------
// fcm_timer.h
// -------------------------------------------------------------------------------------------------

#ifndef FCM_TIMER_H
#define FCM_TIMER_H

#include "app_timer.h"
#include "fcm_interface.h"
#include "fcm_types.h"


typedef struct
{
	FCM_INTERFACE_BASETYPE_FIELDS;
	FCM_MESSAGE_DEF(timeout_ind, app_timer_id_t timer_id; );
} fcm_timer_t;

// -------------------------------------------------------------------------------------------------
// fcm_timer_init
// -------------------------------------------------------------------------------------------------
// p_interface: pointer to the Timer interface.
// -------------------------------------------------------------------------------------------------
void timer_init( fcm_timer_t* p_interface);


#endif
