#include <stdio.h>

long int fact(int c)
{
	if (c == 0)
		return ((long)1);
	else
		return (c * fact(c - 1));
}

main(void)
{
	int n = 0;
	int m = 20;
	double s = 0;
//	printf("Broj clanova niza = ");
//	scanf("%d",&m);
	for (n = 0; n <= m; ++n)
		s = s + 1 / (double)fact(n);
	printf("e = %.17f\n", s);
	return (0);
}