// -------------------------------------------------------------------------------------------------
// StateMachineEngine.c
// -------------------------------------------------------------------------------------------------

#include "StateMachineEngine.h"

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
			StateMachineEngine(pComp, pComp->logical, pComp->logical.Yes);
		}
		else
		{
			StateMachineEngine(pComp, pComp->logical, pComp->logical.No);
		}
  }

  return SMR_Okay;
}
