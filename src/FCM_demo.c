/*
 ============================================================================
 Name        : FCM_demo.c
 Author      : Fred Dijkstra.
 Description : Functional Components Method (FCM) console demo.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "DemoDevice.h"


void printControlMessagesMenu()
{
	//	MESSAGE_DEF(StartInd);
	//	MESSAGE_DEF(PauseInd);
	//	MESSAGE_DEF(RestartInd);
	//	MESSAGE_DEF(QuitInd);

	printf("\n---------------------------------------------\n");
	printf("0. Quit\n");
	printf("1. Control:StartInd\n");
	printf("2. Control:PauseInd\n");
	printf("3. Control:RestartInd\n");
	printf("4. Control:QuitInd\n");
	printf("\nGive choice:");

}

typedef enum
{
  Control_StartInd = 1,
  Control_PauseInd,
  Control_RestartInd,
  Control_QuitInd,
  Command_SpeedInd,
  Command_SteerInd
} TestMessage_t;


// Run as console
int main ()
{
	printf("-- Functional Component Method demo --\n");

	DemoDevice_t demoDevice;

	DemoDevice_init(&demoDevice);


	printControlMessagesMenu();

	// 1. Control:StartInd
	// 2. Control:PauseInd
	// 3. Control:RestartInd
	// 4. Control:QuitInd
	// 5. Command:SpeedInd
	// 6. Command:SteerInd


	// 1. Idle
	Keystroke(&demoDevice.CommandHandler, Control_StartInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	// 2. Running
	Keystroke(&demoDevice.CommandHandler, Control_PauseInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	// 3. Paused
	Keystroke(&demoDevice.CommandHandler, Control_PauseInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	// 4. Running
	Keystroke(&demoDevice.CommandHandler, Command_SpeedInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	// 5.
	Keystroke(&demoDevice.CommandHandler, Control_PauseInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	// 6.
	Keystroke(&demoDevice.CommandHandler, Control_PauseInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	// 7.
	Keystroke(&demoDevice.CommandHandler, Control_PauseInd);
	ProcessMessage((Device_t*)&demoDevice);
	ProcessMessage((Device_t*)&demoDevice);

	printf("-- Ready --\n");

	return EXIT_SUCCESS;
}
