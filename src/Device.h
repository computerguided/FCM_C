// -------------------------------------------------------------------------------------------------
// Device.h
// -------------------------------------------------------------------------------------------------

#ifndef DEVICE_H
#define DEVICE_H

#include "Types.h"
#include "MessageQueue.h"
#include "Component.h"
#include "StateMachineEngine.h"

// The size of the main message queue.
#define MAIN_MESSAGE_QUEUE_SIZE 100

typedef struct
{
	MessageQueue_t* pMainMsgQueue;
} Device_t;

void InitDevice(Device_t *pDevice);
void ProcessDeviceStep(Device_t *pDevice);


#endif
