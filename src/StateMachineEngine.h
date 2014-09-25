// -------------------------------------------------------------------------------------------------
// StateMachineEngine.h
// -------------------------------------------------------------------------------------------------

#ifndef STATE_MACHINE_ENGINE_H
#define STATE_MACHINE_ENGINE_H

#include "Types.h"
#include "Interface.h"
#include "Component.h"

typedef enum
{
	SMR_Okay,
	SMR_Unexpexcted_message
} StateMachineResult_t;

// -------------------------------------------------------------------------------------------------
StateMachineResult_t StateMachineEngine(Component_t* pComp, Interface_t* pInterface, char* pMsgId);




#endif
