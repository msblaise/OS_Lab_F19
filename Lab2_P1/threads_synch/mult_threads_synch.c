#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

// initialize mutex and barrier
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barr;

int SharedVariable = 0;

// function used to check if command line argument is a nonzero integer
bool isInteger(char value[])
{
	int i = 0;
	if(value[0] == '-')
		i = 1;

	for(; value[i] != 0; i++)
	{
		if(!isdigit(value[i]))
			return false;
	}

	return true;
}

void * SimpleThread(void * which)
{
	int num, val;

	for(num = 0; num < 20; num++)
	{
	#ifdef PTHREAD_SYNC
		if(random() > RAND_MAX / 2)
			usleep(10);
	#endif
		// create a lock to prevent other threads incrementing the SharedVariable while
		// one is already executing
		pthread_mutex_lock(&mut);

		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;

		// unlock to allow other threads to start executing
		pthread_mutex_unlock(&mut);
	}

	// create a barrier to allow the threads to synchronize and produce the same final value
	pthread_barrier_wait(&barr);

	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
	
}

int main(int argc, char *argv[])
{
	// too few arguments
	if(argc < 2)
		printf("Too few arguments! Please enter one argument -> [filename arg1]\n");
	
	// too many arguments
	else if(argc > 2)
		printf("Too many arguments! Please enter one argument -> [filename arg1]\n");
	
	// correct amount of arguments
	else
	{
		// first check if the argument is a nonzero integer
		bool val = isInteger(argv[1]);
		if(val == false)
		{
			printf("Error! Please enter a nonzero integer number as an argument\n");
			exit(0);
		}
		
		// start creating threads
		else
		{
			int num_threads = atoi(argv[1]);

			pthread_t threads[num_threads];
			int rc;
			long t;
			
			// initialize the barrier
			if(pthread_barrier_init(&barr, NULL, num_threads))
			{
				printf("Could not create a barrier\n");
				return -1;
			}
			
			for(t = 0; t < num_threads; t++)
			{
				printf("In main: creating thread %ld\n", t);
				rc = pthread_create(&threads[t], NULL, SimpleThread, (void*)t);
				if (rc)
				{
					printf("ERROR; return code from pthread_create() is %d\n", rc);
					exit(-1);
				}
			}
			
			// joining threads suspend the execution of the calling thread until the 
			// target thread finishes executing
			for(t = 0; t < num_threads; t++)
			{
				pthread_join(threads[t], NULL);
			}
			
			// terminate the thread
			pthread_exit(NULL);
		}
	}
	
	return 0;

}
	
