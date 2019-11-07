#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

int SharedVariable = 0;

// function that checks if the command line argument is a nonzero integer
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
		
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;
	}
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
}

int main(int argc, char *argv[])
{
	// too few arguments
	if(argc < 2)
		printf("Too few arguments! Please enter one argument -> [filename arg1]\n");
	// too many argumentss
	else if(argc > 2)
		printf("Too many arguments! Please enter one argument -> [filename arg1]\n");
	// just the right amount of arguments
	else
	{
		// first check if the argument is a nonzero integer 
		bool val = isInteger(argv[1]);
		if(val == false)
		{
			printf("Error! Please enter a nonzero integer number as an argument\n");
			exit(0);
		}
		// start creating threads if the command line is fully validated
		else
		{
			int num_threads = atoi(argv[1]);

			pthread_t threads[num_threads];
			int rc;
			long t;
			
			// thread creation
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
			// terminate the calling thread
			pthread_exit(NULL);
		}
	}
	
	return 0;

}
	
