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
	// -- Find interface --
	SttElement_t* pElement = pComp->pCurrentState++;

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

	while( ((State_t*)pComp->pCurrentState)->pEvaluation != NULL )
	{
		if( ((State_t*)pComp->pCurrentState)->pEvaluation(pComp) )
		{
			StateMachineEngine(pComp, (Interface_t*)&pComp->logical, pComp->logical.pYes_id);
		}
		else
		{
			StateMachineEngine(pComp, (Interface_t*)&pComp->logical, pComp->logical.pNo_id);
		}
  }

  return SMR_Okay;
}
