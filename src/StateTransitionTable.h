// -------------------------------------------------------------------------------------------------
// StateTransition.h
// -------------------------------------------------------------------------------------------------

#ifndef STATE_TRANSITION_TABLE_H
#define STATE_TRANSITION_TABLE_H

#include "Types.h"
#include "StateTransitionTable.h"

typedef enum
{
  Stt_State = 0,
  Stt_Interface,
  Stt_Message,
  Stt_TransitionFunction,
  Stt_NextState
} SttElementType_t;

typedef struct
{
	void* pReference;
	SttElementType_t referenceType;
	void* pNextElement;
} SttElement_t;

void Transition(SttElement_t* pTable, void* pState, void* pInterface, void* pMsg, void *pTransFunct, void* pNextState );

#endif
