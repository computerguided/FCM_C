// -------------------------------------------------------------------------------------------------
// MessageQueue.c
// -------------------------------------------------------------------------------------------------

#include "MessageQueue.h"


#include <string.h> // For memcpy

// -------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------
// MessageQueue_init
// -------------------------------------------------------------------------------------------------
// Initialize the message queue administration.
// -------------------------------------------------------------------------------------------------
// - pMsgQueue : pointer to the created message queue administration.
// - queueSize : the size of the message queue expressed in number of data-less messages.
// -------------------------------------------------------------------------------------------------
void MessageQueue_init(MessageQueue_t* pMsgQueue, int queueSize )
{
  pMsgQueue->pMessage = malloc(sizeof(MessageQueue_t)*queueSize);
  pMsgQueue->pWrite = pMsgQueue->pMessage;
  pMsgQueue->pRead = pMsgQueue->pWrite;
  pMsgQueue->pWrapAround = &pMsgQueue->pMessage[queueSize-1]+1;
  pMsgQueue->pEnd_of_queue = &pMsgQueue->pMessage[queueSize-1]+1;
}


// -------------------------------------------------------------------------------------------------
// PrepareMessage
// -------------------------------------------------------------------------------------------------
// Make room for the new message and get the location in the new message in the message queue
// and set the fields already.
// -------------------------------------------------------------------------------------------------
// - pMsgQueue : pointer to the message queue.
// - pMsgId : message id, implemented as a pointer to a literal string.
// - msgSize : size of the message.
// -------------------------------------------------------------------------------------------------
void* PrepareMessage(MessageQueue_t* pMsgQueu, char* pMsgId, int msgSize)
{
	// Wrap around when there is not enough room.
	if((address_t)pMsgQueu->pEnd_of_queue-(address_t)&pMsgQueu->pWrite->pMsgId < msgSize )
	{
		// Note that when the message is not sent in the transition,
		// this wrap around is done for nothing.
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
		if( pSubMsgQueue->pRead->pInterface != NULL ) // Was message deleted?
		{
			// -- Copy message --

			// Create room in destination message queue.
			PrepareMessage(pMainMsgQueue, pSubMsgQueue->pRead->pMsgId, pSubMsgQueue->pRead->msgSize);

			// Copy the structure
			*pMainMsgQueue->pWrite = *pSubMsgQueue->pRead;

			// Copy the message parameters (includes message id).
			memcpy( &pMainMsgQueue->pWrite->pMsgId, &pSubMsgQueue->pRead->pMsgId, pSubMsgQueue->pRead->msgSize);

			// Now 'send' the message by shifting the write-pointer.
			pMainMsgQueue->pWrite =
					(void*)((address_t)&pMainMsgQueue->pWrite->pMsgId + pMainMsgQueue->pWrite->msgSize);
		}

		// Message from source message-queue 'handled'.
		pSubMsgQueue->pRead =
				(void *)((address_t)&pSubMsgQueue->pRead->pMsgId+pSubMsgQueue->pRead->msgSize); \
		if( pSubMsgQueue->pRead == pSubMsgQueue->pWrapAround )
		{
			pSubMsgQueue->pRead = pSubMsgQueue->pMessage;
		}

		///NEXT_MESSAGE(pSubMsgQueue);
	}
}


