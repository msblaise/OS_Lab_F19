#include <stdio.h>
#include "test_calls.h"

int main(void)
{
	char * one, two, three, four, five, six, seven, eight;

	one = (char*) malloc (10000);
	two = (char*) malloc (20000);
	three = (char*) malloc (30000);
	four = (char*) malloc (40000);
	five = (char*) malloc (50000);
	six = (char*) malloc (60000);
	seven = (char*) malloc (70000);
	eight = (char*) malloc (80000);

	printf("Amount of free bytes: %d\n", syscall(__NR_get_slob_amount_free));
	printf("Amount of claimed bytes: %d\n", syscall(__NR_get_slob_amount_claimed));

	return 0;
}
