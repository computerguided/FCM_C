// -------------------------------------------------------------------------------------------------
// StateTransition.c
// -------------------------------------------------------------------------------------------------

#include "StateTransitionTable.h"

// -------------------------------------------------------------------------------------------------
// Transition
// -------------------------------------------------------------------------------------------------
// Add the given transition to the STT.
// -------------------------------------------------------------------------------------------------
// - index : index of the transition (starts at 1).
// - pTable: pointer to the STT. Already completely allocated, but empty at the first call.
// - pState: pointer to the state of the transition.
// - pInterface: pointer to the interface of the transition.
// - pMsg: pointer to the message of the transition.
// - pTransFunc: pointer to the transition-function of the transition.
// - pNextState: pointer to the next state of the transition.
// -------------------------------------------------------------------------------------------------
void SetTransition(int index, SttElement_t* pTable, void* pState, void* pInterface, void* pMsg, void *pTransFunc, void* pNextState )
{
	// Create a temporary pointer to the location of the new transition.
	SttElement_t* newSttEntry = &pTable[(index-1)*TRANSITION_SIZE];

	newSttEntry[Stt_State].pReference				= pState;
	newSttEntry[Stt_Interface].pReference			= pInterface;
	newSttEntry[Stt_Message].pReference				= pMsg;
	newSttEntry[Stt_TransitionFunction].pReference	= pTransFunc;
	newSttEntry[Stt_NextState].pReference 			= pNextState;

	for(int i=0; i<TRANSITION_SIZE; i++)
	{
		newSttEntry[i].pNextElement = NULL;
	}

	SttElement_t* pStateElement = pTable;

	if( index == 1 )
	{
		// This is the first transition.
		return;
	}

	SttElement_t* pInterfaceElement;
	SttElement_t* pMsgElement;

	// Loop through the states in the table.
	while( pStateElement != NULL )
	{
		if( pStateElement->pReference == pState )
		{
			// State found, find interface.
			pInterfaceElement = pStateElement+1;

			// Loop through the interfaces in the found state.
			while( pInterfaceElement != NULL )
			{
				if( pInterfaceElement->pReference == pInterface )
				{
					// Interface found.
					pMsgElement = pInterfaceElement+1;

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
							pMsgElement->pNextElement = &newSttEntry[Stt_Message];
							return;
						}

						// Move to consecutive message.
						pMsgElement = pMsgElement->pNextElement;
					}

					break; // Interface found.
				}

				if( pInterfaceElement->pNextElement == NULL )
				{
					// This was the last interface in this state and therefore interface not found.
					pInterfaceElement->pNextElement = &newSttEntry[Stt_Interface];
					return;
				}

				// Move to consecutive interface.
				pInterfaceElement = pInterfaceElement->pNextElement;
			}

			break; // State found.
		}

		if( pStateElement->pNextElement == NULL )
		{
			// This was the last state and therefore state not found.
			pStateElement->pNextElement = &newSttEntry[Stt_State];
			return;
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
		pInterface = pState+1;
		while( pInterface != NULL )
		{
			// -- Loop through messages in this interface in this state --

			// Point to first message for this interface.
			pMessage = pInterface+1;
			while( pMessage != NULL )
			{
				// Point to transition function of this message.
				pTransFuncElement = pMessage+1;

				// Set default (i.e. stays like this when dead-state).
				pNextStateElement = pTransFuncElement+1;
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

