// -------------------------------------------------------------------------------------------------
// fcm_state_machine_engine.h
// -------------------------------------------------------------------------------------------------

#ifndef FCM_STATE_MACHINE_ENGINE_H
#define FCM_STATE_MACHINE_ENGINE_H

#include "fcm_component.h"
#include "fcm_interface.h"
#include "fcm_types.h"

typedef enum
{
	FCM_SMR_OK,
	FCM_SMR_UNEXPECTED_MESSAGE
} fcm_state_machine_result_t;

// -------------------------------------------------------------------------------------------------
fcm_state_machine_result_t fcm_state_machine_engine(fcm_component_t* p_component, fcm_interface_t* p_interface, char* p_message_id);




#endif
