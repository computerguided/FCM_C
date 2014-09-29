// -------------------------------------------------------------------------------------------------
// DemoDevice.h
// -------------------------------------------------------------------------------------------------
#ifndef DEMO_DEVICE_H
#define DEMO_DEVICE_H
// -------------------------------------------------------------------------------------------------

#include "Types.h"
#include "Device.h"
#include "Controller.h"
#include "CommandHandler.h"
#include "TimerHandler.h"

#define NUM_MSG_QUEUES 2
typedef struct
{
	int numMsgQueue;
	MessageQueue_t msgQueue[NUM_MSG_QUEUES];
	Controller_t Controller;
	CommandHandler_t CommandHandler;
	TimerHandler_t TimerHandler;
} DemoDevice_t;

void DemoDevice_init(DemoDevice_t* pDevice);


// -------------------------------------------------------------------------------------------------
#endif
