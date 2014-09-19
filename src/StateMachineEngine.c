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

	// -- Find interface --
	pElement = pComp->pCurrentState++;

	while( pElement->pReference != (void *)pInterface )
	{
		pElement = pElement->pNextElement;
		if( pElement == NULL ) return SMR_Unexpexcted_message;
	}

	// -- Find message --
	pElement++;

	while( pElement->pReference != (void *)pMsgId )
	{
		pElement = pElement->pNextElement;
		if( pElement == NULL ) return SMR_Unexpexcted_message;
	}

	// Call the transition function
	pElement++;
	((TransitionFunction_t)pElement->pReference)(pComp);

	// Go to the next state.
	pComp->pCurrentState = pElement->pNextElement;
	pElement = pComp->pCurrentState;

	if( ((State_t*)pElement->pReference)->pEvaluation != NULL )
	{
		if( ((State_t*)pElement->pReference)->pEvaluation(pComp) )
		{
			return StateMachineEngine(pComp, (Interface_t*)&pComp->Logical, pComp->Logical.pYes_id);
		}
		else
		{
			return StateMachineEngine(pComp, (Interface_t*)&pComp->Logical, pComp->Logical.pNo_id);
		}
	}

	return SMR_Okay;
}
