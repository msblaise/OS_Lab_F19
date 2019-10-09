#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

//pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
//pthread_barrier_t barr;
int SharedVariable = 0;

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
		if(random() > RAND_MAX / 2)
			usleep(10);
		//pthread_mutex_lock(&mut);
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;
		//pthread_mutex_unlock(&mut);
	}
	//pthread_barrier_wait(&barr);
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
	
}

int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("Too few arguments! Please enter one argument -> [filename arg1]\n");
	else if(argc > 2)
		printf("Too many arguments! Please enter one argument -> [filename arg1]\n");
	else
	{
		bool val = isInteger(argv[1]);
		if(val == false)
		{
			printf("Error! Please enter a nonzero integer number as an integer\n");
			exit(0);
		}
		else
		{
			int num_threads = atoi(argv[1]);

			pthread_t threads[num_threads];
			int rc;
			long t;
			
			/*if(pthread_barrier_init(&barr, NULL, num_threads))
			{
				printf("Could not create a barrier\n");
				return -1;
			}*/
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
			/*for(t = 0; t < num_threads; t++)
			{
				pthread_join(threads[t], NULL);
			}*/
			pthread_exit(NULL);
			

		}
	}
	
	return 0;

}
	
