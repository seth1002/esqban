// cpp_compiler_options_openmp.cpp
#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include <time.h>
#include <omp.h>	// vcompd.dll has no manifest
//#include <crtdbg.h>




int main(void)
{
	printf("%d processors available\n", omp_get_num_procs());

	double start = omp_get_wtime( );
	int i;
	#pragma omp parallel for
	for(i=0; i<500000000; i++)
	{
		float f = (float)i / 2;
	}
	double end = omp_get_wtime( );

	printf("%.8f seconds.\n", (end - start));

	printf("\nPress enter...\n");
	getchar();

	// 1 CPU: 1.64901377
	// 2 CPU: 0.90883869

	return 0;
}

/*
void main()
{
	#pragma omp parallel
	{
		printf("Hello World\n");
	}
}
*/

/*
volatile DWORD dwStart;
volatile int global = 0;

double test2(int num_steps) {
	int i;
	global++;
	double x, pi, sum = 0.0, step;

	step = 1.0 / (double) num_steps;

#pragma omp parallel for reduction(+:sum) private(x)
	for (i = 1; i <= num_steps; i++) {
		x = (i - 0.5) * step;
		sum = sum + 4.0 / (1.0 + x*x);
	}

	pi = step * sum;
	return pi;
}

int main(int argc, char* argv[]) {
	double   d;
	int n = 1000000;

	if (argc > 1)
		n = atoi(argv[1]);

	dwStart = GetTickCount();
	d = test2(n);
	printf("For %d steps, pi = %.15f, %d milliseconds\n", n, d, GetTickCount() - dwStart);

	dwStart = GetTickCount();
	d = test2(n);
	printf("For %d steps, pi = %.15f, %d milliseconds\n", n, d, GetTickCount() - dwStart);
}
*/
