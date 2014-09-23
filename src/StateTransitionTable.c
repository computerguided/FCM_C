// -------------------------------------------------------------------------------------------------
// StateTransition.c
// -------------------------------------------------------------------------------------------------

#include "StateTransitionTable.h"


// -------------------------------------------------------------------------------------------------
// CopyTransition
// -------------------------------------------------------------------------------------------------
// Copy the 5 elements that together comprise the transition.
// -------------------------------------------------------------------------------------------------
// - pFrom: pointer to first element in temporary array of 5-referenceType elements.
// - pTo: pointer to the first element in the STT where the transition must be copied.
// -------------------------------------------------------------------------------------------------
void CopyTransition(SttElement_t* pFrom, SttElement_t* pTo)
{
	SttElement_t* p_from = pFrom;
	SttElement_t* p_to = pTo;

	for(int i=0; i<5-(int)pTo->referenceType; i++)
	{
		*p_to = *p_from;
		p_to++;
		p_from++;
	}
}

// -------------------------------------------------------------------------------------------------
// AppendNewEntry
// -------------------------------------------------------------------------------------------------
// Append a new entry to supplied element.
// -------------------------------------------------------------------------------------------------
// - pNewElement: pointer to first element in temporary array of 5-entryType elements.
// - pLastElement: pointer to the element to which the new entry must be attached.
// -------------------------------------------------------------------------------------------------
void AppendNewEntry(SttElement_t* pNewElement, SttElement_t* pLastElement)
{
	SttElement_t* p_lastElement = pLastElement;

	p_lastElement->pNextElement = p_lastElement+(5-(int)pLastElement->referenceType);
	CopyTransition( pNewElement, p_lastElement->pNextElement );
}


// -------------------------------------------------------------------------------------------------
// Transition
// -------------------------------------------------------------------------------------------------
// Add the given transition to the STT.
// -------------------------------------------------------------------------------------------------
// - pTable: pointer to the STT. Already completely allocated, but empty at the first call.
// - pState: pointer to the state of the transition.
// - pInterface: pointer to the interface of the transition.
// - pMsg: pointer to the message of the transition.
// - pTransFunc: pointer to the transition-function of the transition.
// - pNextState: pointer to the next state of the transition.
// -------------------------------------------------------------------------------------------------
void SetTransition(SttElement_t* pTable, void* pState, void* pInterface, void* pMsg, void *pTransFunc, void* pNextState )
{
	// Create a temporary structure holding the elements referring to the supplied state, interface, message
	// and next state.
	SttElement_t newSttEntry[TRANSITION_SIZE] =
	{
			{pState, Stt_State, NULL},
			{pInterface, Stt_Interface, NULL},
			{pMsg, Stt_Message, NULL},
			{pTransFunc, Stt_TransitionFunction, NULL},
			{pNextState, Stt_NextState, NULL}
	};

	SttElement_t* pStateElement = pTable;
	SttElement_t* pInterfaceElement;
	SttElement_t* pMsgElement;

	// Loop through the states in the table.
	while( pStateElement != NULL )
	{
		if( pStateElement->pReference == pState )
		{
			// State found, find interface.
			pInterfaceElement = pStateElement++;

			// Loop through the interfaces in the found state.
			while( pInterfaceElement != NULL )
			{
				if( pInterfaceElement->pReference == pInterface )
				{
					// Interface found.
					pMsgElement = pInterfaceElement++;

					// Loop through the messages of the found interface to
					// check whether message not already handled.
					while( pMsgElement != NULL )
					{
						if( pMsgElement->pReference == pMsg )
						{
							// Message found --> double message --> ERROR
							// Transition not added.
							// TODO: Handle feedback for this error.
							return; // Message found.
						}

						if( pMsgElement->pNextElement == NULL )
						{
							// This was the last message in this state/interface and therefore message
							// not found, which is how it should be at this point.
							AppendNewEntry( newSttEntry, pMsgElement );
						}

						// Move to consecutive message.
						pMsgElement = pMsgElement->pNextElement;
					}

					break; // Interface found.
				}

				if( pInterfaceElement->pNextElement == NULL )
				{
					// This was the last interface in this state and therefore interface not found.
					AppendNewEntry( newSttEntry, pInterfaceElement );
				}

				// Move to consecutive interface.
				pInterfaceElement = pInterfaceElement->pNextElement;
			}

			break; // State found.
		}

		if( pStateElement->pNextElement == NULL )
		{
			// This was the last state and therefore state not found.
			AppendNewEntry( newSttEntry, pStateElement );
		}

		// Move to consecutive state.
		pStateElement = pStateElement->pNextElement;
	}
}

// -------------------------------------------------------------------------------------------------
// SetNextState
// -------------------------------------------------------------------------------------------------
// When all the transitions are added, all 'next-states' are temporarily assumed to be
// dead-states. This must be corrected for those next-states that are not dead-states.
// -------------------------------------------------------------------------------------------------
// - pTable: pointer to the (first element) of the STT.
// -------------------------------------------------------------------------------------------------
void SetNextState(SttElement_t* pTable )
{
	SttElement_t* pTransFuncElement = pTable;
	SttElement_t* pStateElement = pTable;
	SttElement_t* pNextStateElement;

	SttElement_t* pState = pTable;
	SttElement_t* pInterface;
	SttElement_t* pMessage;

	// Loop through the table and find and process transition-function elements.

	while( pState != NULL )
	{
		// -- Loop through interfaces in this state --

		// Point to first interface in this state.
		pInterface = pState++;
		while( pInterface != NULL )
		{
			// -- Loop through messages in this interface in this state --

			// Point to first message for this interface.
			pMessage = pInterface++;
			while( pMessage != NULL )
			{
				// Point to transition function of this message.
				pTransFuncElement = pMessage++;

				// Set default (i.e. stays like this when dead-state).
				pNextStateElement = pTransFuncElement++;
				pTransFuncElement->pNextElement = pNextStateElement;

				// -- Loop through all states --

				pStateElement = pTable;
				while( pStateElement != NULL )
				{
					if( pStateElement->pReference == pNextStateElement->pReference)
					{
						// Elements reference the same state.
						pTransFuncElement->pNextElement = pStateElement;
						break;
					}
					// Move to consecutive state.
					pStateElement = pStateElement->pNextElement;
				}
				// Move to consecutive message.
				pMessage = pMessage->pNextElement;
			}
			// Move to consecutive interface.
			pInterface = pInterface->pNextElement;
		}
		// Move to consecutive state.
		pState = pState->pNextElement;
	}
}

