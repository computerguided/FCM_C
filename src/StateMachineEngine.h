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
// -- StateMachineEngine --
// -------------------------------------------------------------------------------------------------
// Process the message.
// - pComp: pointer to the component that received the message.
// - pInterface: pointer to the interface of the component on which the message is received.
// - pMsgId: unique message id, i.e. pointer to literal string.
// Returns:
// - SMR_Okay: when message properly handled.
// - SMR_Unexpected_message: when not.
// -------------------------------------------------------------------------------------------------
StateMachineResult_t StateMachineEngine(Component_t* pComp, Interface_t* pInterface, char* pMsgId);




#endif
