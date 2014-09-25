// -------------------------------------------------------------------------------------------------
// DemoDevice.c
// -------------------------------------------------------------------------------------------------

#include "DemoDevice.h"

void DemoDevice_init(DemoDevice_t* pDevice)
{
	//	Initialize the message queues.
	pDevice->numMsgQueue = NUM_MSG_QUEUES;
	INIT_MSG_QUEUE(0,10);
	INIT_MSG_QUEUE(1,10);

	//	Initialize the components.
	INIT_COMPONENT(Controller);
	INIT_COMPONENT(CommandHandler);

	//	Connect the interfaces.
	CONNECT_INTERFACES(Command,CommandHandler,Controller);
}
