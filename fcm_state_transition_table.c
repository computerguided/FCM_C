// -------------------------------------------------------------------------------------------------
// fcm_state_transition.c
// -------------------------------------------------------------------------------------------------

#include "fcm_state_transition_table.h"

#include <assert.h>


// -------------------------------------------------------------------------------------------------
void fcm_set_stt_elements(fcm_stt_element_t* p_table, uint32_t num_elements)
{
	for( uint32_t i=0;i<num_elements;i++)
	{
		p_table[i].p_next_element = NULL;
		p_table[i].p_reference    = NULL;
	}
}


// -------------------------------------------------------------------------------------------------
fcm_stt_element_t* fcm_get_empty_element(fcm_stt_element_t* p_table, uint32_t buffer_size, uint32_t num_elements)
{
	fcm_stt_element_t* p_element = NULL;

	uint32_t i;
	for( i=0; i<num_elements; i++)
	{
		if( p_table[i].p_next_element == NULL && p_table[i].p_reference == NULL)
		{
			p_element = &p_table[i];
			break;
		}
	}

	assert(p_element != NULL && num_elements-i >= buffer_size);
	return p_element;
}



// -------------------------------------------------------------------------------------------------
// fcm_create_transition
// -------------------------------------------------------------------------------------------------
// Create a new transition.
// Returns the pointer to the first fcm_stt_element_t of the created transition.
// -------------------------------------------------------------------------------------------------
// - level : whether the first element is a state, interface or message.
// - p_table : pointer to the table of fcm_stt_element_t elements.
// - num_elements : number of elements in the p_table.
// - p_state: pointer to the state of the transition.
// - p_interface: pointer to the interface of the transition.
// - p_message: pointer to the message of the transition.
// - p_transition_function: pointer to the transition-function of the transition.
// - p_next_state: pointer to the next state of the transition.
// -------------------------------------------------------------------------------------------------

fcm_stt_element_t* fcm_create_transition
(
		fcm_stt_reference_t 	level,
		fcm_stt_element_t*			p_table,
		uint32_t   					num_elements,
		void* 						p_state,
		void* 						p_interface,
		void* 						p_message,
		void* 						p_transition_function,
		void* 						p_next_state
)
{
	uint32_t num_entries = FCM_TRANSITION_SIZE-(uint32_t)level;

	// Create the necessary elements for the new transition.
	fcm_stt_element_t* new_stt_entry = fcm_get_empty_element(p_table, num_entries, num_elements);

	// -- Set the reference depending on what must be set --

	if( level == FCM_STT_STATE ) 		new_stt_entry[FCM_STT_STATE].p_reference = p_state;
	if( level <= FCM_STT_INTERFACE ) 	new_stt_entry[FCM_STT_INTERFACE-level].p_reference = p_interface;
	if( level <= FCM_STT_MESSAGE ) 		new_stt_entry[FCM_STT_MESSAGE-level].p_reference = p_message;
										new_stt_entry[FCM_STT_TRANSITION_FUNCTION-level].p_reference = p_transition_function;

	new_stt_entry[FCM_STT_TRANSITION_FUNCTION-level].p_next_element = p_next_state; // Temp.

	return new_stt_entry;
}


// -------------------------------------------------------------------------------------------------
// fcm_set_transition
// -------------------------------------------------------------------------------------------------
// Add the given transition to the STT.
// Returns the pointer to the first transition. This is done to be able to allocate the first.
// -------------------------------------------------------------------------------------------------
// - index : index of the transition (starts at 1).
// - p_table: pointer to the first transition.
// - num_elements : number of elements in the p_table.
// - p_state: pointer to the state of the transition.
// - p_interface: pointer to the interface of the transition.
// - p_message: pointer to the message of the transition.
// - p_transition_function: pointer to the transition-function of the transition.
// - p_next_state: pointer to the next state of the transition.
// -------------------------------------------------------------------------------------------------

fcm_stt_element_t* fcm_set_transition
(
		uint32_t 			index,
		fcm_stt_element_t* 	p_table,
		uint32_t   			num_elements,
		void* 				p_state,
		void* 				p_interface,
		void* 				p_message,
		void* 				p_transition_function,
		void*				p_next_state
)
{
	// If this is the first transition.
	if( index == 1 ) return
			fcm_create_transition(FCM_STT_STATE, p_table, num_elements, p_state, p_interface, p_message, p_transition_function, p_next_state );

	fcm_stt_element_t* p_stt_element = p_table;
	fcm_stt_element_t* pInterfaceElement;
	fcm_stt_element_t* pMsgElement;

	// Loop through the states in the table.
	while( p_stt_element != NULL )
	{
		if( p_stt_element->p_reference == p_state )
		{
			// State found, find interface.
			pInterfaceElement = p_stt_element+1;

			// Loop through the interfaces in the found state.
			while( pInterfaceElement != NULL )
			{
				if( pInterfaceElement->p_reference == p_interface )
				{
					// Interface found.
					pMsgElement = pInterfaceElement+1;

					// Loop through the messages of the found interface to
					// check whether message not already handled.
					while( pMsgElement != NULL )
					{
						if( pMsgElement->p_reference == p_message )
						{
							// Message found --> double message --> ERROR
							// Transition not added.
							// TODO: Handle feedback for this error.
							return p_table; // Message found.
						}

						if( pMsgElement->p_next_element == NULL )
						{
							// This was the last message in this state/interface and therefore message
							// not found, which is how it should be at this point.
							pMsgElement->p_next_element = fcm_create_transition(FCM_STT_MESSAGE, p_table, num_elements, p_state, p_interface, p_message, p_transition_function, p_next_state );
							return p_table;
						}

						// Move to consecutive message.
						pMsgElement = pMsgElement->p_next_element;
					}

					break; // Interface was found.
				}

				if( pInterfaceElement->p_next_element == NULL )
				{
					// This was the last interface in this state and therefore interface not found.
					pInterfaceElement->p_next_element = fcm_create_transition(FCM_STT_INTERFACE, p_table, num_elements, p_state, p_interface, p_message, p_transition_function, p_next_state );
					return p_table;
				}

				// Move to consecutive interface.
				pInterfaceElement = pInterfaceElement->p_next_element;
			}

			break; // State was found.
		}

		if( p_stt_element->p_next_element == NULL )
		{
			// This was the last state and therefore state not found.
			p_stt_element->p_next_element = fcm_create_transition(FCM_STT_STATE, p_table, num_elements, p_state, p_interface, p_message, p_transition_function, p_next_state );
			return p_table;
		}

		// Move to consecutive state.
		p_stt_element = p_stt_element->p_next_element;
	}
	return p_table;
}

// -------------------------------------------------------------------------------------------------
// fcm_set_next_states
// -------------------------------------------------------------------------------------------------
// When all the transitions are added, all 'next-states' are temporarily assumed to be
// dead-states. This must be corrected for those next-states that are not dead-states.
// -------------------------------------------------------------------------------------------------
// - p_table: 		pointer to the (first element) of the STT.
// - num_elements: 	number of elements in the p_table.
// -------------------------------------------------------------------------------------------------
void fcm_set_next_states(fcm_stt_element_t* p_table, uint32_t num_elements )
{
	fcm_stt_element_t* p_transition_function_element	= p_table;
	fcm_stt_element_t* p_stt_element 				= p_table;

	fcm_stt_element_t* p_state = p_table;
	fcm_stt_element_t* p_interface;
	fcm_stt_element_t* p_message;

	// Loop through the table and find and process transition-function elements.

	while( p_state != NULL )
	{
		// -- Loop through interfaces in this state --

		// Point to first interface in this state.
		p_interface = p_state+1;
		while( p_interface != NULL )
		{
			// -- Loop through messages in this interface in this state --

			// Point to first message for this interface.
			p_message = p_interface+1;
			while( p_message != NULL )
			{
				// Point to transition function of this message.
				p_transition_function_element = p_message+1;

				// -- Loop through all states --

				p_stt_element = p_table;
				while( p_stt_element != NULL )
				{
					if( p_stt_element->p_reference == p_transition_function_element->p_next_element)
					{
						// State found.
						p_transition_function_element->p_next_element = p_stt_element;
						break;
					}
					// Move to consecutive state.
					p_stt_element = p_stt_element->p_next_element;
				}

				if( p_stt_element == NULL )
				{
					// State not found.
					fcm_stt_element_t* p_next_state = fcm_get_empty_element(p_table, 1, num_elements);
					p_next_state->p_reference = p_transition_function_element->p_next_element;
					p_next_state->p_next_element = NULL;
					p_transition_function_element->p_next_element = p_next_state;
				}

				// Move to consecutive message.
				p_message = p_message->p_next_element;
			}
			// Move to consecutive interface.
			p_interface = p_interface->p_next_element;
		}
		// Move to consecutive state.
		p_state = p_state->p_next_element;
	}
}

