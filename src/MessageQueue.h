// -------------------------------------------------------------------------------------------------
// MessageQueue.h
// -------------------------------------------------------------------------------------------------

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "Types.h"
#include "Interface.h"
#include "Utils.h"

typedef struct
{
	uint32_t systemTime;
	Interface_t* interface;
	int msgSize;
	char* msgId;
} Message_t;

typedef struct
{
	Message_t* pMessage;
	Message_t* pWrite;
	Message_t* pRead;
	Message_t* pWrapAround;
	void* pEnd_of_queue;
} MessageQueue_t;

#define CREATE_MESSAGE_QUEUE(m,s) \
MessageQueue_t m; \
MessageQueueElement_t m##_queue[s]; \
m.pMessage = &m##_queue; \
m.pMessage = &m##_queue; \
m.pEnd_of_queue = (void *)(&m##_queue[s-1]+1); \
m.pWrapAround = m.pEnd_of_queue;

#define MESSAGE_SIZE(m) (int)(&m->eom)-(int)(&m->pId)

void prepare_message(MessageQueue_t* q, void* m, char* pId, int s);

#define PREPARE_MESSAGE(i,m) \
prepare_message(comp.msgQueue, comp.i.m, comp.i.m##_id, MESSAGE_SIZE(comp.i.m))

#define SEND_MESSAGE(i,m) \
comp->pMsgQueue.pWrite->interface = comp->i.pRemoteInterface; \
comp->pMsgQueue.pWrite->systemTime = GetSystemTime(); \
comp->pMsgQueue.pWrite = &comp.pMsgQueue.pWrite->pId + comp->pMsgQueue.pWrite->msgSize; \
comp->i.m = NULL

#endif
