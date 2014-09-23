// -------------------------------------------------------------------------------------------------
// MessageQueue.c
// -------------------------------------------------------------------------------------------------

#include "MessageQueue.h"


#include <string.h> // For memcpy

// -------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------
// -- PrepareMessage --
// -------------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------------
void* PrepareMessage(MessageQueue_t* pMsgQueu, char* pMsgId, int msgSize)
{
	if((int)pMsgQueu->pEnd_of_queue-(int)&pMsgQueu->pWrite->pMsgId < msgSize )
	{
		pMsgQueu->pWrapAround = pMsgQueu->pWrite;
		pMsgQueu->pWrite = pMsgQueu->pMessage;
	}
	pMsgQueu->pWrite->pMsgId = pMsgId;
	pMsgQueu->pWrite->msgSize = msgSize;
	return &pMsgQueu->pWrite->pMsgId;
}

// -------------------------------------------------------------------------------------------------
// CopyMessages
// -------------------------------------------------------------------------------------------------
// Copy all the messages from one message queue into the other.
// -------------------------------------------------------------------------------------------------
// - pSubMsgQueue : pointer to the message queue from which the message must be copied.
// - pMainMsgQueue : pointer to the message queue to which the message must be copied.
// -------------------------------------------------------------------------------------------------
void CopyMessages(MessageQueue_t* pSubMsgQueue, MessageQueue_t* pMainMsgQueue)
{
	// Check whether there is a message to copy.
	while( pSubMsgQueue->pRead != pSubMsgQueue->pWrite )
	{
		// -- Copy message --

		PrepareMessage(pMainMsgQueue, pSubMsgQueue->pRead->pMsgId, pSubMsgQueue->pRead->msgSize);

		// Copy the structure
		*pMainMsgQueue->pWrite = *pSubMsgQueue->pRead;

		// Copy the message parameters (includes message id).
		memcpy( &pMainMsgQueue->pWrite->pMsgId, &pSubMsgQueue->pRead->pMsgId, pSubMsgQueue->pRead->msgSize);

		NEXT_MESSAGE(pSubMsgQueue);
	}
}
