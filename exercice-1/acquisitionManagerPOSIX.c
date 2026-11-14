#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include "acquisitionManager.h"
#include "msg.h"
#include "iSensor.h"
#include "multitaskingAccumulator.h"
#include "iAcquisitionManager.h"
#include "debug.h"


#define BUFFER_SIZE 1024
//Buffer
MSG_BLOCK buffer[BUFFER_SIZE];
unsigned int readindex = 0;
unsigned int writeindex = 0;

//producer count storage
volatile unsigned int produceCount = 0;

//Thread
pthread_t producers[4];

static void *produce(void *params);

/**
* Semaphores and Mutex
*/
//TODO (done en theorie)
#define SEM_LIBRE_NAME "/semaphorelibre"
#define SEMAPHORE_INITIAL_LIBRE_VALUE 1024
#define SEM_OCCUPE_NAME "/semaphoreoccupe"
#define SEMAPHORE_INITIAL_OCCUPE_VALUE 0

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
sem_t *semaphorelibre; ///Initialisé à la taille du buffer (1024)
sem_t *semaphoreoccupe; //Initialisé à 0 car buffer vide

/*
* Creates the synchronization elements.
* @return ERROR_SUCCESS if the init is ok, ERROR_INIT otherwise
*/
static unsigned int createSynchronizationObjects(void);

/*
* Increments the produce count.
*/
static void incrementProducedCount(void);

static unsigned int createSynchronizationObjects(void)
{

	//TODO(done)
	
    sem_unlink(SEM_LIBRE_NAME);
    semaphorelibre = sem_open(SEM_LIBRE_NAME, O_CREAT, 0644, SEMAPHORE_INITIAL_LIBRE_VALUE);
	sem_unlink(SEM_OCCUPE_NAME);
    semaphoreoccupe = sem_open(SEM_OCCUPE_NAME, O_CREAT, 0644, SEMAPHORE_INITIAL_OCCUPE_VALUE);

    if (semaphoreoccupe == SEM_FAILED || semaphorelibre == SEM_FAILED)
    {
        perror("[sem_open");
        return ERROR_INIT;
    }

	printf("[acquisitionManager]Semaphore created\n");
	return ERROR_SUCCESS;
}

static void incrementProducedCount(void)
{
	//TODO(done)
	pthread_mutex_lock(&m1);
	produceCount ++;
	pthread_mutex_unlock(&m1);
}

unsigned int getProducedCount(void)
{
	unsigned int p = 0;
	//TODO(done)
	pthread_mutex_lock(&m1);
	p = produceCount;
	pthread_mutex_unlock(&m1);
	return p;
}

MSG_BLOCK getMessage(void){
	//TODO
	MSG_BLOCK message;
	sem_wait(semaphoreoccupe);
	message = buffer[readindex];
	readindex =(readindex + 1)%BUFFER_SIZE;
	sem_post(semaphorelibre);
}

//TODO create accessors to limit semaphore and mutex usage outside of this C module.
static void putMessage(MSG_BLOCK *message){
	sem_wait(semaphorelibre);
	pthread_mutex_lock(&m1);
	buffer[writeindex] = *message;
	writeindex = (writeindex + 1)%BUFFER_SIZE;
	pthread_mutex_unlock(&m1);
}

unsigned int acquisitionManagerInit(void)
{
	unsigned int i;
	printf("[acquisitionManager]Synchronization initialization in progress...\n");
	fflush( stdout );
	if (createSynchronizationObjects() == ERROR_INIT)
		return ERROR_INIT;
	
	printf("[acquisitionManager]Synchronization initialization done.\n");

	for (i = 0; i < PRODUCER_COUNT; i++)
	{
		//TODO(done)
		pthread_create(&producers[i], NULL, produce, NULL);
	}

	return ERROR_SUCCESS;
}

void acquisitionManagerJoin(void)
{
	unsigned int i;
	for (i = 0; i < PRODUCER_COUNT; i++)
	{
		//TODO(done)
		pthread_join(producers[i], NULL);
	}

	//TODO(done)
	sem_close(semaphorelibre);
	sem_close(semaphoreoccupe);
	sem_unlink("/semaphorelibre");
	sem_unlink("/semaphoreoccupe");
	printf("[acquisitionManager]Semaphore cleaned\n");
}

void *produce(void* params)
{
	D(printf("[acquisitionManager]Producer created with id %d\n", gettid()));
	MSG_BLOCK message;
	unsigned int i = 0;
	while (i < PRODUCER_LOOP_LIMIT)
	{
		i++;
		sleep(PRODUCER_SLEEP_TIME+(rand() % 5));
		//TODO(done)
		getInput(i, &message);
		putMessage(&message);
	}
	printf("[acquisitionManager] %d termination\n", gettid());
	//TODO(done)
	return NULL;
}