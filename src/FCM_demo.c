/*
 ============================================================================
 Name        : FCM_demo.c
 Author      : Fred Dijkstra - Computerguided Systems B.V.
 Description : Functional Components Method (FCM) console demo.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "Device.h"


int main(void)
{
	puts("Functional Components Demo\n");

	Device_t demoDevice;
	InitDevice(&demoDevice);

	getchar();


	puts("Bye");

	return EXIT_SUCCESS;
}


/*
#include <pthread.h>
#define NUM_THREADS     5

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   pthread_exit(NULL);
}
*/
