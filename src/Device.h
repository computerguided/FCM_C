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
  MessageQueue_t* msgQueue;
} Device_t;

// -------------------------------------------------------------------------------------------------
bool ProcessMessage(Device_t *pDevice);

// -------------------------------------------------------------------------------------------------
// Initialization macros
// -------------------------------------------------------------------------------------------------

#define INIT_MSG_QUEUE(i,s) \
		MessageQueue_init(&pDevice->msgQueue[i], s)

#define INIT_COMPONENT(c) \
		c##_init(&pDevice->c)

#define CONNECT_INTERFACES(i,c_left,c_right) \
		ConnectInterface(&pDevice->c_left.i, &pDevice->c_right.i)

// -------------------------------------------------------------------------------------------------
#endif
