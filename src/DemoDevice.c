// -------------------------------------------------------------------------------------------------
 // DemoDevice.c
// -------------------------------------------------------------------------------------------------

#include <stdio.h>

#include "DemoDevice.h"

void DemoDevice_init(DemoDevice_t* pDevice)
{
	//	Initialize the message queues.
	pDevice->numMsgQueues = NUM_MSG_QUEUES;
	INIT_MSG_QUEUE(0,10);
	INIT_MSG_QUEUE(1,4);
	INIT_MSG_QUEUE(2,4);
	pDevice->pMsgQueue = pDevice->msgQueue;

	//	Initialize the components (supply index of message queue).
	INIT_COMPONENT(Controller,0);
	INIT_COMPONENT(CommandHandler,1);
	INIT_COMPONENT(TimerHandler,2);

	//	Connect the interfaces.
	CONNECT_INTERFACES(Command,CommandHandler,Controller);
	CONNECT_INTERFACES(Control,CommandHandler,Controller);

	// Connect components to the Timer Handler.
	CONNECT_TIMER(Controller,TimerHandler);

}
