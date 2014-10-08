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

// Run as console
int main ()
{
	printf("-- Functional Component Method demo --\n");

	DemoDevice_t demoDevice;

	DemoDevice_init(&demoDevice);

	// Send start indication.

	for( int i=0; i<3; i++ )
	{
		Keystroke(&demoDevice.CommandHandler, i);

		if( ProcessMessage((Device_t*)&demoDevice) )
		{
			printf("Succes\n");
		}
		else
		{
			printf("Failure\n");
		}
	}


	printf("-- Ready --\n");

	return EXIT_SUCCESS;
}
