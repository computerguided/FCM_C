// -------------------------------------------------------------------------------------------------
// StateTransitionTable.h
// -------------------------------------------------------------------------------------------------

#ifndef STATE_TRANSITION_TABLE_H
#define STATE_TRANSITION_TABLE_H

#include "Types.h"

typedef enum
{
  Stt_State = 0,
  Stt_Interface,
  Stt_Message,
  Stt_TransitionFunction,
  Stt_NextState
} SttReferenceType_t;


typedef struct
{
	void* pReference;
	SttReferenceType_t referenceType;
	void* pNextElement;
} SttElement_t;

void Transition(SttElement_t* pTable, void* pState, void* pInterface, void* pMsg, void *pTransFunct, void* pNextState );

#define TRANSITION_SIZE 5

#endif
