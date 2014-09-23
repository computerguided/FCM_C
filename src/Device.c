// -------------------------------------------------------------------------------------------------
// Device.c
// -------------------------------------------------------------------------------------------------

#include "Device.h"

// -------------------------------------------------------------------------------------------------
// -- InitDevice --
// -------------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------------
void InitDevice(Device_t *pDevice)
{
	// -- Construct static message queues --
	CREATE_MESSAGE_QUEUE(mainMsgQueue, MAIN_MESSAGE_QUEUE_SIZE);

	// Link to device.
	pDevice->pMainMsgQueue = &mainMsgQueue;

	// -- Construct components --
}

// -------------------------------------------------------------------------------------------------
// -- ProcessDeviceStep --
// -------------------------------------------------------------------------------------------------
// - pDevice : pointer to the device.
// -------------------------------------------------------------------------------------------------
void ProcessDeviceStep(Device_t* pDevice)
{
	// -- Copy messages out of sub message queue(s) --
	// CopyMessages( pDevice->pSubMsgQueue, pDevice->pMainMsgQueue);

	// There is at least one message pending when the read pointer is not equal
	// to the write pointer.
	if( pDevice->pMainMsgQueue->pRead != pDevice->pMainMsgQueue->pWrite )
	{
		// Handle pending message by calling the state machine engine and
		// supplying the component, interface and message.
		// Assert when the message was not found.
		assert( StateMachineEngine(
				(Component_t*)pDevice->pMainMsgQueue->pRead->pInterface->pComponent,
				pDevice->pMainMsgQueue->pRead->pInterface,
				pDevice->pMainMsgQueue->pRead->pMsgId ) != SMR_Okay );
		NEXT_MESSAGE(pDevice->pMainMsgQueue);
	}
}



