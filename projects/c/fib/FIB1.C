#include <stdio.h>

void main(void)
{
	int i;
	unsigned int f[200];

	f[0] = 0;
	f[1] = 1;

	for(i=2; i<200; i++)
	{
		f[i] = f[i-1] + f[i-2];
		printf("f[%d]=%d\n", i, f[i]);
	}

	printf("f[99]=%d\n", f[99]);
	printf("f[199]=%d\n", f[199]);
}

