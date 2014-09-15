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
	CREATE_MESSAGE_QUEUE(mainMsgQueue, 100);

	// Link to device.
	pDevice->pMainMsgQueue = &mainMsgQueue;

	// -- Construct components --
}

// -------------------------------------------------------------------------------------------------
// -- ProcessDeviceStep --
// -------------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------------
void ProcessDeviceStep(Device_t *pDevice)
{
	// CopyMessage( pDevice->pSubMsgQueue, pDevice->pMainMsgQueue);

	MessageQueue_t* pMsgQueue = pDevice->pMainMsgQueue;

	if( pMsgQueue->pRead != pMsgQueue->pWrite )
	{
		// Handle pending message.
		if( StateMachineEngine(
				(Component_t*)pMsgQueue->pRead->pInterface->pComponent,
				pMsgQueue->pRead->pInterface,
				pMsgQueue->pRead->pMsgId ) != SMR_Okay )
		{
		    //ErrorHandler("Unknown message");
		}

		NEXT_MESSAGE(pMsgQueue);
	}
}



