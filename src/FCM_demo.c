/*
 ============================================================================
 Name        : FCM_demo.c
 Author      : Fred Dijkstra - Computerguided Systems B.V.
 Description : Functional Components Method (FCM) console demo.
 ============================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "Device.h"


int main(void)
{
  char *m,*b,*w,*l,*d,*a,*p;
  int option;
  struct termios oldt, newt;

  /* tcgetattr gets the parameters of the current terminal
   * STDIN_FILENO will tell tcgetattr that it should write the settings
   * of stdin to oldt
   */
  tcgetattr( STDIN_FILENO, &oldt);
  /*now the settings will be copied*/
  memcpy((void *)&newt, (void *)&oldt, sizeof(struct termios));

  newt.c_lflag &= ~(ICANON);  // Reset ICANON so enter after char is not needed
  newt.c_lflag &= ~(ECHO);    // Turn echo off

  /*
   *  Those new settings will be set to STDIN
   *  TCSANOW tells tcsetattr to change attributes immediately.
   */
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);

  m="\t\t Main Menu \t\t";
  b="------------------------";
  w="1- Word Perfect.";
  l="2- Lotus 1-2-3.";
  d="3- dBase IV.";
  a="4- AutoCAD.";
  p="Press the required number:";

  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s ",m,b,w,l,d,a,b,p);
  option=getchar(); //this is where the getch is used
  printf("\nYour choice is: %d\n",option);

  option=getchar(); //this is where the getch is used
  printf("\nYour choice is: %d\n",option);

  option=getchar(); //this is where the getch is used
  printf("\nYour choice is: %d\n",option);

  /*
   *  Restore original settings
   */
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

  printf("\033[2J"); /*clear screen*/
  printf("\033[2;1H"); /*put in seperator between User input and chat text*/
  printf("-----------------------------\n"); /*barrier*/
  printf("\033[1;1H"); /*put at upper left*/
  printf(">some user text not done yet... "); /*user is in the middle of typing*/

  printf("\033[3;1H"); /*move to 2nd line on screen*/
  printf("\033[1L"); /*insert line here*/
  printf("comment one\n"); /*some text arrives*/
  printf("\033[1;1H"); /*go back to first row*/
  printf("\033[4~"); /*supposed to be "END" key but doesnt work (hitting the end key doesnt seem to work either, so I dont know what to put here)*/
  printf("more text... ");

  printf("\033[3;1H"); /*move to 2nd line on screen*/
  printf("\033[1L"); /*insert line here*/
  printf("comment two\n"); /*some more text arrives*/
  printf("\033[1;1H"); /*go back to first row*/
  printf("\033[4~"); /*supposed to be "END" key but doesnt work (hitting the end key doesnt seem to work either, so I dont know what to put here)*/


}





//int main(void)
//{
//	puts("Functional Components Demo\n");
//
//	Device_t demoDevice;
//	InitDevice(&demoDevice);
//
//	puts("Bye");
//
//	return EXIT_SUCCESS;
//}
//
//
///*
//#include <pthread.h>
//#define NUM_THREADS     5
//
//void *PrintHello(void *threadid)
//{
//   long tid;
//   tid = (long)threadid;
//   printf("Hello World! It's me, thread #%ld!\n", tid);
//   pthread_exit(NULL);
//}
//
//int main (int argc, char *argv[])
//{
//   pthread_t threads[NUM_THREADS];
//   int rc;
//   long t;
//   for(t=0; t<NUM_THREADS; t++){
//      printf("In main: creating thread %ld\n", t);
//      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
//      if (rc){
//         printf("ERROR; return code from pthread_create() is %d\n", rc);
//         exit(-1);
//      }
//   }
//
//   pthread_exit(NULL);
//}
//*/
