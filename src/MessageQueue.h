// -------------------------------------------------------------------------------------------------
// MessageQueue.h
// -------------------------------------------------------------------------------------------------

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "Types.h"
#include "Interface.h"
#include "Utils.h"

// -------------------------------------------------------------------------------------------------
typedef struct
{
	uint32_t systemTime;
	Interface_t* pInterface;
	int msgSize;
	char* pMsgId;
} Message_t;

// -------------------------------------------------------------------------------------------------
typedef struct
{
	Message_t* pMessage;
	Message_t* pWrite;
	Message_t* pRead;
	void* pWrapAround;
	void* pEnd_of_queue;
} MessageQueue_t;


// -------------------------------------------------------------------------------------------------
#define MESSAGE_SIZE(m) \
		(int)(&m->eom)-(int)(&m->pMsgId)

// -------------------------------------------------------------------------------------------------
#define NEXT_MESSAGE(q) \
		q->pRead = (void *)((address_t)&q->pRead->pMsgId+q->pRead->msgSize); \
		if( q->pRead == q->pWrapAround ) q->pRead = q->pMessage

// -------------------------------------------------------------------------------------------------
void MessageQueue_init(MessageQueue_t* pMsgQueue, int queueSize );

// -------------------------------------------------------------------------------------------------
void* PrepareMessage(MessageQueue_t* pMsgQueu, char* pMsgId, int msgSize);

// -------------------------------------------------------------------------------------------------
void CopyMessages(MessageQueue_t* pSubMsgQueue, MessageQueue_t* pMainMsgQueue);

#endif
