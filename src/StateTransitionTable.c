// -------------------------------------------------------------------------------------------------
// StateTransition.c
// -------------------------------------------------------------------------------------------------

#include "StateTransitionTable.h"

void CopyTransition(SttElement_t* pFrom, SttElement_t* pTo);
void AppendNewEntry(SttElement_t* pNewElement, SttElement_t* pLastElement);



// -------------------------------------------------------------------------------------------------
// -- AppendNewEntry --
// -------------------------------------------------------------------------------------------------
// Append a new entry to supplied element.
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
// -- CopyTransition --
// -------------------------------------------------------------------------------------------------
// Copy the 5 elements that together comprise the transition.
// - pFrom: pointer to first element in temporary array of 5-entryType elements.
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
// -- Transition --
// -------------------------------------------------------------------------------------------------
// Add the given transition to the STT.
// - pTable: pointer to the STT. Already completely allocated, but empty at the first call.
// - pState: pointer to the state of the transition.
// - pInterface: pointer to the interface of the transition.
// - pMsg: pointer to the message of the transition.
// - pTransFunc: pointer to the transition-function of the transition.
// - pNextState: pointer to the next state of the transition.
// -------------------------------------------------------------------------------------------------
void Transition(SttElement_t* pTable, void* pState, void* pInterface, void* pMsg, void *pTransFunc, void* pNextState )
{
	SttElement_t newSttEntry[5] =
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
					// Interface found, go to last message and check message.
					pMsgElement = pInterfaceElement++;

					// Loop through the messages of the found interface to
					// check messages to see if message not already added.
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

						pMsgElement = pMsgElement->pNextElement;
					}

					break; // Interface found.
				}

				if( pInterfaceElement->pNextElement == NULL )
				{
					// This was the last interface in this state and therefore interface not found.
					AppendNewEntry( newSttEntry, pInterfaceElement );
				}

				pInterfaceElement = pInterfaceElement->pNextElement;
			}

			break; // State found.
		}

		if( pStateElement->pNextElement == NULL )
		{
			// This was the last state and therefore state not found.
			AppendNewEntry( newSttEntry, pStateElement );
		}

		pStateElement = pStateElement->pNextElement;
	}
}

// -------------------------------------------------------------------------------------------------
// -- SetNextState --
// -------------------------------------------------------------------------------------------------
// When all the transitions are added, all 'next-states' are temporarily assumed to be
// dead-states. This must be corrected for those next-states that are not dead-states.
// - pTable: pointer to the (first element) of the STT.
// -------------------------------------------------------------------------------------------------
void SetNextState(SttElement_t* pTable )
{
	SttElement_t* pTransFuncElement = pTable;
	SttElement_t* pStateElement = pTable;
	SttElement_t* pNextStateElement;

	while( pTransFuncElement->pReference != NULL )
	{
		if( pTransFuncElement->referenceType == Stt_TransitionFunction )
		{
			pNextStateElement = pTransFuncElement+1;
			pTransFuncElement->pNextElement = pNextStateElement;

			pStateElement = pTable;
			while( pStateElement != NULL )
			{
				if( pStateElement->referenceType == Stt_State &&
						pStateElement->pReference == pNextStateElement->pReference)
				{
					pTransFuncElement->pNextElement = pStateElement;
					break;
				}
				pStateElement++;
			}
		}
		pTransFuncElement++;
	}
}
