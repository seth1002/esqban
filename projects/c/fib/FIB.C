#include <stdio.h>

void main(void)
{
	register unsigned int a=0, b=1, c=0;

	while(c<200){
		a = a+b;
		b = b+a;
		c++;
		printf("a=%d\nb=%d\n", a, b);	
	}

	printf("%d\n", b);	

	return b;
}