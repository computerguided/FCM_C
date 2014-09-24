// -------------------------------------------------------------------------------------------------
// Device.h
// -------------------------------------------------------------------------------------------------

#ifndef DEVICE_H
#define DEVICE_H

#include "Types.h"
#include "MessageQueue.h"
#include "Component.h"
#include "StateMachineEngine.h"

typedef struct
{
	MessageQueue_t* pMainMsgQueue;
} Device_t;

void InitDevice(Device_t *pDevice);
void ProcessDeviceStep(Device_t *pDevice);

#endif
