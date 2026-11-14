#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "displayManager.h"
#include "iDisplay.h"
#include "iAcquisitionManager.h"
#include "iMessageAdder.h"
#include "msg.h"
#include "multitaskingAccumulator.h"
#include "debug.h"

// DisplayManager thread.
pthread_t displayThread;

/**
 * Display manager entry point.
 * */
static void *display( void *parameters );


void displayManagerInit(void){
	//TODO (done)
	pthread_create(&displayThread, NULL, display, NULL); //A verifier
}

void displayManagerJoin(void){
	//TODO (done)	
	pthread_join(&displayThread,NULL);
} 

static void *display( void *parameters )
{
	D(printf("[displayManager]Thread created for display with id %d\n", gettid()));
	unsigned int diffCount = 0;
	MSG_BLOCK s;
	while(diffCount < DISPLAY_LOOP_LIMIT){
		sleep(DISPLAY_SLEEP_TIME);
		//TODO (là je ne comprends pas)
	}
	printf("[displayManager] %d termination\n", gettid());
   //TODO(done)
   return NULL;
}