// -------------------------------------------------------------------------------------------------
// MessageQueue.c
// -------------------------------------------------------------------------------------------------

#include "MessageQueue.h"

// -------------------------------------------------------------------------------------------------

void prepare_message(MessageQueue_t* q, void* m, char* pId, int s)
{
	if((int)q->pEnd_of_queue-(int)&q->pWrite->msgId < s )
	{
		q->pWrapAround = q->pWrite;
		q->pWrite = q->pMessage;
	}
	m = &q->pWrite->msgId;
	q->pWrite->msgId = pId;
	q->pWrite->msgSize = s;
}

