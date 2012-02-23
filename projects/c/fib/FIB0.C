#include <stdio.h>

unsigned int fib(unsigned int n)
{
	if(n == 0)
		return 0;
	if(n == 1)
		return 1;
	printf("n=%d\n", n);
	return fib(n-1) + fib(n-2);
}

void main(void)
{
	printf("%d\n", fib((unsigned int)200));
}

