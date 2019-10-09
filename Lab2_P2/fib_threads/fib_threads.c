#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * fibonacci(void *param); // threads call this function
int * fib; // this data is shared by threads

// fibonacci function
void * fibonacci (void * param)
{
	int i, n = atoi(param);

	// first and second elements are 0 and 1 respectively
	fib[0] = 0;
	fib[1] = 1;

	for (i = 2; i <= n; i++)
	{
		fib[i] = fib[i-1] + fib[i-2];
	}
	
	// thread termination
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	// thread identifier
	pthread_t tid;
	
	// set of thread attributess
	pthread_attr_t attr;

	int fib_count = atoi(argv[1]);

	// allocate memory for shared fib array
	fib = (int *)malloc(fib_count * sizeof(int));

	// incorrect number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}

	// if argument is negative, return error
	if (fib_count < 0)
	{
		fprintf(stderr, "%d must be >= 0\n", fib_count);
	}

	// get the default attributes
	pthread_attr_init(&attr);
			
	// create the thread
	pthread_create(&tid, &attr, fibonacci, argv[1]);

	// wait for thread to exit
	pthread_join(tid, NULL);
			
	int i;

	// print out the elements of the shared fib array
	for(i = 0; i < fib_count; i++)
	{
		printf("%d\n", fib[i]);
	}   
	
	return 0;

}
	
