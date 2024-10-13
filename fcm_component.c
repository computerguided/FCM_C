// -------------------------------------------------------------------------------------------------
// fcm_component.c
// -------------------------------------------------------------------------------------------------

#include "fcm_component.h"

// -------------------------------------------------------------------------------------------------
// fcm_set_first_state
// -------------------------------------------------------------------------------------------------
// After the state transition table is setup properly, the first state can be set. This is done by
// looping through the state transition table and find that element that references the specified
// state.
// -------------------------------------------------------------------------------------------------
// Parameters:
// - p_component: pointer to the component.
// - p_state: pointer to the first state.
// -------------------------------------------------------------------------------------------------
void fcm_set_first_state(fcm_component_t* p_component, fcm_state_t* p_state)
{
	p_component->p_current_state = NULL;

	// Set the loop variable to the first element in the table, which is the first state to examine.
	fcm_stt_element_t* p_state_element = p_component->p_transitions;

	while( p_state_element != NULL )
	{
		if( (fcm_state_t *)p_state_element->p_reference == p_state )
		{
			// Element found that references to the first state.
			p_component->p_current_state = p_state_element;
			break;
		}
		// Go to the next state.
		p_state_element = p_state_element->p_next_element;
	}
	// End of the table reached.

	// Assert when the state is not found. Probably missing a transition.
	assert(p_component->p_current_state != NULL);
}

