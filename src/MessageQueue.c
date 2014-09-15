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
// -- CopyMessage --
// -------------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------------
void CopyMessage(MessageQueue_t* pSubMsgQueue, MessageQueue_t* pMainMsgQueue)
{

	void* pNewMsg;

	while( pSubMsgQueue->pRead != pSubMsgQueue->pWrite )
	{
		// Copy message
		pNewMsg = PrepareMessage(pMainMsgQueue, pSubMsgQueue->pRead->pMsgId, pSubMsgQueue->pRead->msgSize);
		pMainMsgQueue->pWrite = pSubMsgQueue->pRead;

		memcpy( &pMainMsgQueue->pWrite->pMsgId, &pSubMsgQueue->pRead->pMsgId, pSubMsgQueue->pRead->msgSize);

		NEXT_MESSAGE(pSubMsgQueue);
	}
}
