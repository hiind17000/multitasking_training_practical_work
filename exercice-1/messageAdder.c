#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h>
#include <pthread.h>
#include "messageAdder.h"
#include "msg.h"
#include "iMessageAdder.h"
#include "multitaskingAccumulator.h"
#include "iAcquisitionManager.h"
#include "debug.h"

//consumer thread
pthread_t consumer;
//Message computed
volatile MSG_BLOCK out;
//Consumer count storage
volatile unsigned int consumeCount = 0;

//Mutex
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

/**
 * Increments the consume count.
 */
static void incrementConsumeCount(void);

/**
 * Consumer entry point.
 */
static void *sum( void *parameters );

static void incrementProducedCount(void)
{
	//TODO(done)
	pthread_mutex_lock(&m2);
	consumeCount ++;
	pthread_mutex_unlock(&m2);
}

MSG_BLOCK getCurrentSum(){
	//TODO(done)
	MSG_BLOCK somme;
	pthread_mutex_lock(&m2);
	somme = out;
	pthread_mutex_unlock(&m2);
	return somme;
}

unsigned int getConsumedCount(){
	//TODO(done)
	unsigned int c = 0;
	pthread_mutex_lock(&m2);
	c = consumeCount;
	pthread_mutex_unlock(&m2);
	return c;
}

void messageAdderInit(void){
	out.checksum = 0;
	for (size_t i = 0; i < DATA_SIZE; i++)
	{
		out.mData[i] = 0;
	}
	//TODO(done)
	pthread_create(&consumer, NULL, sum, NULL);
}

void messageAdderJoin(void){
	//TODO(done)
	pthread_join(consumer, NULL);
}

static void *sum( void *parameters )
{
	D(printf("[messageAdder]Thread created for sum with id %d\n", gettid()));
	unsigned int i = 0;
	MSG_BLOCK m;
	while(i<ADDER_LOOP_LIMIT){
		i++;
		sleep(ADDER_SLEEP_TIME);
		//TODO(done)
		m = getMessage();
		if (messageCheck(&m)) {             // Exigencie 3: checksum
			pthread_mutex_lock(&m2);
			messageAdd(&out, &m);
			incrementConsumeCount();
			pthread_mutex_unlock(&m2);
		} else {
			printf("[messageAdder]ERROR: corrupted message dropped\n");
		}
	}
	printf("[messageAdder] %d termination\n", gettid());
	//TODO(done)
	return NULL
}


