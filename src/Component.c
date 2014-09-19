// -------------------------------------------------------------------------------------------------
// Component.c
// -------------------------------------------------------------------------------------------------

#include "Component.h"

// -------------------------------------------------------------------------------------------------
// -- SetFirstState --
// -------------------------------------------------------------------------------------------------
// After the transition table is setup properly, the first state can be set.
// - pComp: pointer to the component.
// - pState: pointer to the first state.
// -------------------------------------------------------------------------------------------------

void SetFirstState(Component_t* pComp, State_t* pState)
{
	SttElement_t* pStateElement = pComp->pTransitions;
	while( pStateElement != NULL )
	{
		if( pStateElement->referenceType == Stt_State &&
				pStateElement->pReference == pState )
		{
			pComp->pCurrentState = pStateElement;
			break;
		}
		pStateElement++;
	}
}

