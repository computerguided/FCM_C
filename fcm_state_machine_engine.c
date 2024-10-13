// -------------------------------------------------------------------------------------------------
// fcm_state_machine_engine.c
// -------------------------------------------------------------------------------------------------

#include "error_handler.h"

#include "fcm_state_machine_engine.h"

// -------------------------------------------------------------------------------------------------
// -- fcm_state_machine_engine --
// -------------------------------------------------------------------------------------------------
// Process the message.
// - p_component: pointer to the component that received the message.
// - p_interface: pointer to the interface of the component on which the message is received.
// - p_message_id: unique message id, i.e. pointer to literal string.
// -------------------------------------------------------------------------------------------------
//#define SME_VERBOSE
fcm_state_machine_result_t fcm_state_machine_engine(fcm_component_t* p_component, fcm_interface_t* p_interface, char* p_message_id)
{
	#ifdef SME_VERBOSE
		NRF_LOG_INFO ("State machine processing message '%s' on interface '%s' for component '%s'", p_message_id, p_interface->p_name, p_component->p_name);
		NRF_LOG_FLUSH ();
	#endif

	fcm_stt_element_t* p_stt_element;

	// By definition, the element at the next index of an element referencing a
	// state references to an interface.

	// -- Find interface --
	p_stt_element = p_component->p_current_state+1;

	#ifdef SME_VERBOSE
		NRF_LOG_INFO ("Find interface '%s' for component '%s' in state '%s'", p_interface->p_name, p_component->p_name, ((fcm_state_t *)p_component->p_current_state->p_reference)->p_name);
		NRF_LOG_FLUSH ();
	#endif

	while( p_stt_element->p_reference != (void *)p_interface )
	{
		// Shift to the next interface.
		p_stt_element = p_stt_element->p_next_element;
		if( p_stt_element == NULL )
		{
		    NRF_LOG_INFO("ERROR: Message %s on interface %s, but no messages defined on this interface for component %s in state %s",
                    p_message_id, p_interface->p_name, p_component->p_name,
                    ((fcm_state_t *)p_component->p_current_state->p_reference)->p_name);
			NRF_LOG_FLUSH();

			error_handler_set_error(VP_ERROR_CATEGORY_FCM, VP_FCM_UNEXPECTED_MESSAGE);

			return FCM_SMR_UNEXPECTED_MESSAGE;
		}
	}

	// If we're here, the interface was found.

	// By definition, the element at the next index of an element referencing a
	// interface references to a message.

	// -- Find message --
	p_stt_element++;

	#ifdef SME_VERBOSE
		NRF_LOG_INFO ("Find message '%s' on interface '%s' for component '%s' in state '%s'", p_message_id, p_interface->p_name, p_component->p_name, ((fcm_state_t *)p_component->p_current_state->p_reference)->p_name);
		NRF_LOG_FLUSH ();
	#endif

	while( p_stt_element->p_reference != (void *)p_message_id )
	{
		// Shift to the next message.
		p_stt_element = p_stt_element->p_next_element;
		if( p_stt_element == NULL )
		{
            NRF_LOG_INFO("ERROR: Message %s on interface %s for component %s, but message invalid in state %s",
                    p_message_id, p_interface->p_name, p_component->p_name,
                    ((fcm_state_t *)p_component->p_current_state->p_reference)->p_name);
			NRF_LOG_FLUSH();

			error_handler_set_error(VP_ERROR_CATEGORY_FCM, VP_FCM_UNEXPECTED_MESSAGE);

			return FCM_SMR_UNEXPECTED_MESSAGE;
		}
	}

	// If we're here, the message was found. By definition, the element at the next index
	// of an element referencing a message references to an transition-function.

	// Call the transition function (or rather the wrapper).
	p_stt_element++;
	((fcm_transition_function_t)p_stt_element->p_reference)(p_component);

	// Go to the next state.
	p_component->p_current_state = p_stt_element->p_next_element;
	p_stt_element = p_component->p_current_state;

	#ifdef SME_VERBOSE
		NRF_LOG_INFO ("Component '%s' now in state '%s'", p_component->p_name, ((fcm_state_t*)p_component->p_current_state->p_reference)->p_name);
		NRF_LOG_FLUSH ();
	#endif

	if( ((fcm_state_t*)p_stt_element->p_reference)->p_evaluation != NULL )
	{
		// This is a choice-point, for which the transitions are handled 'outside' the message queue.
		if( ((fcm_state_t*)p_stt_element->p_reference)->p_evaluation(p_component) )
		{
			#ifdef SME_VERBOSE
				NRF_LOG_INFO ("Choicepoint '%s' for component '%s' evaluated to Yes", ((fcm_state_t*)p_stt_element->p_reference)->p_name, p_component->p_name);
				NRF_LOG_FLUSH ();
			#endif

			return fcm_state_machine_engine(p_component, (fcm_interface_t*)&p_component->logical, p_component->logical.p_yes_id);
		}
		else
		{
			#ifdef SME_VERBOSE
				NRF_LOG_INFO ("choicepoint '%s' for component '%s' evaluated to No", ((fcm_state_t*)p_stt_element->p_reference)->p_name, p_component->p_name);
				NRF_LOG_FLUSH ();
			#endif

			return fcm_state_machine_engine(p_component, (fcm_interface_t*)&p_component->logical, p_component->logical.p_no_id);
		}
	}

	// If we're here, the last state was not a choice-point.
	return FCM_SMR_OK;
}
