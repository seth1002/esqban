/* gcc -fopenmp -o omp omp.c */
#include <stdio.h>
#include <omp.h>
main()
{
//	#pragma omp parallel
//	printf ("[%d] Hello\n", omp_get_thread_num());

	printf ("%d\n", omp_get_num_procs());

	int i;
	#pragma parallel for
	for(i=0; i<50000000; i++)
	{
		float f = (float)i / 2.f;
	}
}
