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
		if( pDevice->pMsgQueue[i].pRead != pDevice->pMsgQueue[i].pWrite )
		{
			// At least one message pending in this sub-queue.
			CopyMessages(&pDevice->pMsgQueue[i], pDevice->pMsgQueue);
		}
	}

	bool result = true;

	// There is at least one message pending when the read pointer is not equal
	// to the write pointer.
	while( pDevice->pMsgQueue->pRead != pDevice->pMsgQueue->pWrite )
	{
		if( pDevice->pMsgQueue->pRead->pInterface != NULL ) // Was message deleted?
		{
			// Handle pending message by calling the state machine engine and
			// supplying the component, interface and message.
			result = (StateMachineEngine(
					(Component_t*)pDevice->pMsgQueue->pRead->pInterface->pComponent,
					pDevice->pMsgQueue->pRead->pInterface,
					pDevice->pMsgQueue->pRead->pMsgId ) == SMR_Okay );

			NEXT_MESSAGE(pDevice->pMsgQueue);
			return result; // Handle one message per loop.
		}
		NEXT_MESSAGE(pDevice->pMsgQueue);
	}

	return result;
}



