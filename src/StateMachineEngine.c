// -------------------------------------------------------------------------------------------------
// StateMachineEngine.c
// -------------------------------------------------------------------------------------------------

#include "StateMachineEngine.h"

// -------------------------------------------------------------------------------------------------
// -- StateMachineEngine --
// -------------------------------------------------------------------------------------------------
// Process the message.
// - pComp: pointer to the component that received the message.
// - pInterface: pointer to the interface of the component on which the message is received.
// - pMsgId: unique message id, i.e. pointer to literal string.
// -------------------------------------------------------------------------------------------------
StateMachineResult_t StateMachineEngine(Component_t* pComp, Interface_t* pInterface, char* pMsgId)
{
	SttElement_t* pElement;

	// By definition, the element at the next index of an element referencing a
	// state references to an interface.

	// -- Find interface --
	pElement = pComp->pCurrentState+1;

	while( pElement->pReference != (void *)pInterface )
	{
		// Shift to the next interface.
		pElement = pElement->pNextElement;
		if( pElement == NULL ) return SMR_Unexpexcted_message;
	}

	// If we're here, the interface was found.

	// By definition, the element at the next index of an element referencing a
	// interface references to a message.

	// -- Find message --
	pElement++;

	while( pElement->pReference != (void *)pMsgId )
	{
		// Shift to the next message.
		pElement = pElement->pNextElement;
		if( pElement == NULL ) return SMR_Unexpexcted_message;
	}

	// If we're here, the message was found. By definition, the element at the next index
	// of an element referencing a message references to an transition-function.

	// Call the transition function (or rather the wrapper).
	pElement++;
	((TransitionFunction_t)pElement->pReference)(pComp);

	// Go to the next state.
	pComp->pCurrentState = pElement->pNextElement;
	pElement = pComp->pCurrentState;

	if( ((State_t*)pElement->pReference)->pEvaluation != NULL )
	{
		// This is a choicepoint, for which the transitions are handled 'outside' the message queue.
		if( ((State_t*)pElement->pReference)->pEvaluation(pComp) )
		{
			return StateMachineEngine(pComp, (Interface_t*)&pComp->Logical, pComp->Logical.pYes_id);
		}
		else
		{
			return StateMachineEngine(pComp, (Interface_t*)&pComp->Logical, pComp->Logical.pNo_id);
		}
	}

	// If we're here, the last state was not a choicepoint.
	return SMR_Okay;
}
