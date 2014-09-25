// -------------------------------------------------------------------------------------------------
// Device.c
// -------------------------------------------------------------------------------------------------

#include "Device.h"

// -------------------------------------------------------------------------------------------------
// -- ProcessDeviceStep --
// -------------------------------------------------------------------------------------------------
// - pDevice : pointer to the device.
// -------------------------------------------------------------------------------------------------
bool ProcessMessage(Device_t* pDevice)
{
	// -- Copy messages out of sub message queue(s) --
	for(int i=1;i<pDevice->numMsgQueues;i++)
	{
	  CopyMessages(&pDevice->msgQueue[i], pDevice->msgQueue);
	}

	bool result;
	// There is at least one message pending when the read pointer is not equal
	// to the write pointer.
	if( pDevice->msgQueue->pRead != pDevice->msgQueue->pWrite )
	{
		// Handle pending message by calling the state machine engine and
		// supplying the component, interface and message.
		result = (StateMachineEngine(
				(Component_t*)pDevice->msgQueue->pRead->pInterface->pComponent,
				pDevice->msgQueue->pRead->pInterface,
				pDevice->msgQueue->pRead->pMsgId ) != SMR_Okay );

		NEXT_MESSAGE(pDevice->msgQueue);
	}

	return result;
}



