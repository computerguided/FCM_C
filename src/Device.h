// -------------------------------------------------------------------------------------------------
// Device.h
// -------------------------------------------------------------------------------------------------
#ifndef DEVICE_H
#define DEVICE_H
// -------------------------------------------------------------------------------------------------


#include "Types.h"
#include "MessageQueue.h"
#include "Component.h"
#include "StateMachineEngine.h"

typedef struct
{
	int numMsgQueues;
	MessageQueue_t* pMsgQueue;
} Device_t;

#define DEVICE_BASETYPE_FIELDS \
		int numMsgQueues; \
		MessageQueue_t* pMsgQueue

// -------------------------------------------------------------------------------------------------
bool ProcessMessage(Device_t *pDevice);

// -------------------------------------------------------------------------------------------------
// Initialization macros
// -------------------------------------------------------------------------------------------------

#define INIT_MSG_QUEUE(i,s) \
		static Message_t _messages_belonging_to_message_queue_##i[s]; \
		pDevice->msgQueue[i].pMessage = _messages_belonging_to_message_queue_##i; \
		MessageQueue_init(&pDevice->msgQueue[i], s)

#define INIT_COMPONENT(c,x) \
		c##_init(&pDevice->c,&pDevice->msgQueue[x])

#define CONNECT_INTERFACES(i,c_left,c_right) \
		ConnectInterfaces((Interface_t*)&pDevice->c_left.i, (Interface_t*)&pDevice->c_right.i)

#define CONNECT_TIMER(c,t) \
		pDevice->c.Timer.pRemoteInterface = (Interface_t *)&pDevice->t.Timer


// -------------------------------------------------------------------------------------------------
#endif
