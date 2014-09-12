// -------------------------------------------------------------------------------------------------
// State.h
// -------------------------------------------------------------------------------------------------

#ifndef STATE_H
#define STATE_H

#include "Types.h"
#include "Component.h"

typedef bool (*EvaluationFunction_t)(Component_t*);

typedef struct
{
	char* pName;
	EvaluationFunction_t pEvaluation;
} State_t;

#define SET_STATE(s,e) \
s.pName = #s; \
s.pEvaluation = e

#endif
