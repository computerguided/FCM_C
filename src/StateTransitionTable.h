// -------------------------------------------------------------------------------------------------
// StateTransitionTable.h
// -------------------------------------------------------------------------------------------------

#ifndef STATE_TRANSITION_TABLE_H
#define STATE_TRANSITION_TABLE_H

#include "Types.h"

// -------------------------------------------------------------------------------------------------
typedef enum
{
  Stt_State = 0,
  Stt_Interface,
  Stt_Message,
  Stt_TransitionFunction,
} SttReferenceType_t;

// -------------------------------------------------------------------------------------------------
typedef struct
{
	void* pReference;
	void* pNextElement;
} SttElement_t;

// -------------------------------------------------------------------------------------------------
SttElement_t* SetTransition(int index, SttElement_t* pTable, void* pState, void* pInterface, void* pMsg, void *pTransFunct, void* pNextState );

void SetNextStates(SttElement_t* pTable );

#define TRANSITION_SIZE 4
// -------------------------------------------------------------------------------------------------
#endif
