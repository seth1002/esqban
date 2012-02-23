#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define TASKQ_DEPTH 8  

int fib(int n, int d);

fib_taskq(int n, int d) {
    int x, y;
    #pragma intel omp taskq
    {
      #pragma intel omp task
	{
	    x = fib(n - 1, d+1);
	}
	#pragma intel omp task
	{
	    y = fib(n - 2, d+1);
	}
    }
  return x+y;
}

int fib(int n, int d) {
     if (n < 2) {
	  return (n);
     } else if (d < TASKQ_DEPTH) {
	  return fib_taskq(n,d);
     } else {
	  int x, y;
          x = fib(n - 1, d+1);
          y = fib(n - 2, d+1);
	  return (x + y);
     }
}

int fib_iter(int n) {
     int f0, f1, f2, i;
     if (n < 2)
	  return n;
     f0 = 0;
     f1 = 1;
     f2 = 1;
     for (i = 2; i <= n; ++i) {
	  f2 = f0 + f1;
	  f0 = f1;
	  f1 = f2;
     }

     return f2;
}

int main(int argc, char *argv[]) {
     int n, answer;
     double start, end;

     if (argc < 2) {
          fprintf(stderr, "Recommend computing fib 45 to start\n");
          fprintf(stderr, "time requirement roughly doubles for each succeedingfib number\n");
          fprintf(stderr, "Usage: fib n\n");
          return -1;    
     }
     n = atoi(argv[1]);

     #pragma omp parallel
         #pragma omp master
	     printf("Threads:      %d\n", omp_get_num_threads());
	 
#ifdef _OPENMP
     printf("Taskq Depth:  %d\n", TASKQ_DEPTH);
#endif

     start = omp_get_wtime( );
     #pragma omp parallel
     {
         #pragma intel omp taskq
             #pragma intel omp task
                 answer = fib(n,0);
     }

     end = omp_get_wtime( );

     printf("fib(%d) = %d\n", n, answer );
     if (answer != fib_iter(n))
	  printf("WRONG ANSWER!\n");

     printf("Compute Time: %f seconds\n", end - start);

     return 0;
}

