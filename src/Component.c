// -------------------------------------------------------------------------------------------------
// Component.c
// -------------------------------------------------------------------------------------------------

#include "Component.h"

// -------------------------------------------------------------------------------------------------
// -- SetFirstState --
// -------------------------------------------------------------------------------------------------
// After the state transition table is setup properly, the first state can be set. This is done by
// looping through the state transition table and find that element that referecenses the specified
// state.
// Parameters:
// - pComp: pointer to the component.
// - pState: pointer to the first state.
// -------------------------------------------------------------------------------------------------
void SetFirstState(Component_t* pComp, State_t* pState)
{
	pComp->pCurrentState = NULL;

	// Set the loop variable to the first element in the table, which is the first state to examine.
	SttElement_t* pStateElement = pComp->pTransitions;

	while( pStateElement != NULL )
	{
		if( pStateElement->pReference == (void*)pState )
		{
			// Element found that references to the first state.
			pComp->pCurrentState = pStateElement;
			break;
		}
		// Go to the next state.
		pStateElement = pStateElement->pNextElement;
	}
	// End of the table reached.

	// Assert when the state is not found. Probably missing a transition.
	assert(pComp->pCurrentState == NULL);
}

