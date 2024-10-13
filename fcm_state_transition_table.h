// -------------------------------------------------------------------------------------------------
// fcm_state_transition_table.h
// -------------------------------------------------------------------------------------------------

#ifndef FCM_STATE_TRANSITION_TABLE_H
#define FCM_STATE_TRANSITION_TABLE_H

#include "fcm_types.h"

// -------------------------------------------------------------------------------------------------
typedef enum
{
  FCM_STT_STATE = 0,
  FCM_STT_INTERFACE,
  FCM_STT_MESSAGE,
  FCM_STT_TRANSITION_FUNCTION,
} fcm_stt_reference_t;

// -------------------------------------------------------------------------------------------------
typedef struct
{
	void* p_reference;
	void* p_next_element;
} fcm_stt_element_t;

// -------------------------------------------------------------------------------------------------
fcm_stt_element_t* fcm_set_transition(uint32_t index, fcm_stt_element_t* p_table, uint32_t num_elements, void* p_state, void* p_interface, void* p_message, void *p_transition_function, void* p_next_state );

void fcm_set_stt_elements(fcm_stt_element_t* p_table, uint32_t num_elements);

void fcm_set_next_states(fcm_stt_element_t* p_table, uint32_t num_elements );

#define FCM_TRANSITION_SIZE 4
// -------------------------------------------------------------------------------------------------
#endif
