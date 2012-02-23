// OpenMP.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <omp.h>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <windows.h>
#include <intrin.h>
using namespace std;

#include "performance_measure.h"

#define NUM_THREADS 4
#define NUM_START 1
#define NUM_END 10

void test(int val)
{
    #pragma omp parallel if (val) num_threads(val)
    if (omp_in_parallel())
        #pragma omp single
        printf_s("val = %d, parallelized with %d threads\n",
                 val, omp_get_num_threads());
    else
        printf_s("val = %d, serialized\n", val);
}

void AnotherTest( void )
{
   int i, nRet = 0, nSum = 0, nStart = NUM_START, nEnd = NUM_END;
   int nThreads = 0, nTmp = nStart + nEnd;
   unsigned uTmp = (unsigned((abs(nStart - nEnd) + 1)) *
                               unsigned(abs(nTmp))) / 2;
   int nSumCalc = uTmp;

   if (nTmp < 0)
      nSumCalc = -nSumCalc;

   omp_set_num_threads(NUM_THREADS);

   #pragma omp parallel default(none) private(i) shared(nSum, nThreads, nStart, nEnd)
   {
      #pragma omp master
      nThreads = omp_get_num_threads();

      #pragma omp for
      for (i=nStart; i<=nEnd; ++i) {
            #pragma omp atomic
            nSum += i;
      }
   }

   if  (nThreads == NUM_THREADS) {
      printf_s("%d OpenMP threads were used.\n", NUM_THREADS);
      nRet = 0;
   }
   else {
      printf_s("Expected %d OpenMP threads, but %d were used.\n",
               NUM_THREADS, nThreads);
      nRet = 1;
   }

   if (nSum != nSumCalc) {
      printf_s("The sum of %d through %d should be %d, "
               "but %d was reported!\n",
               NUM_START, NUM_END, nSumCalc, nSum);
      nRet = 1;
   }
   else
      printf_s("The sum of %d through %d is %d\n",
               NUM_START, NUM_END, nSum);

}

void test2(int iter)
{
    #pragma omp ordered
    printf_s("test2() iteration %d by thread ID %d\n", iter, omp_get_thread_num());
}

void AnotherTest2( void )
{
    int i;
    #pragma omp parallel
    {
        #pragma omp for ordered
        for (i = 0 ; i < 5 ; i++)
            test2(i);
    }

}

/*
 * taylor.c
 *
 * This program calculates the value of e*pi by first calculating e
 * and pi by their taylor expansions and then multiplying them
 * together.
 */

#define num_steps 20000000

void sequential_taylor( void )
{
  double start, stop; /* times of beginning and end of procedure */
  double e, pi, factorial, product;
  int i;

  printf("Sequential Taylor\n");
  /* start the timer */
  start = clock();

  /* First we calculate e from its taylor expansion */
  printf("e started\n");
  e = 1;
  factorial = 1; /* rather than recalculating the factorial from
		    scratch each iteration we keep it in this varialbe
		    and multiply it by i each iteration. */
  for (i = 1; i<num_steps; i++) {
    factorial *= i;
    e += 1.0/factorial;
  }
  printf("e done\n");

  /* Then we calculate pi from its taylor expansion */
  printf("pi started\n");

  pi = 0;
  for (i = 0; i < num_steps*10; i++) {
    /* we want 1/1 - 1/3 + 1/5 - 1/7 etc.
       therefore we count by fours (0, 4, 8, 12...) and take
         1/(0+1) =  1/1
       - 1/(0+3) = -1/3
         1/(4+1) =  1/5
       - 1/(4+3) = -1/7 and so on */
    pi += 1.0/(i*4.0 + 1.0);
    pi -= 1.0/(i*4.0 + 3.0);
  }
  pi = pi * 4.0;
  printf("pi done\n");

    product = e * pi;

  stop = clock();

  printf("Reached result %f in %.3f seconds\n", product, (stop-start)/1000);

}

void parallel_taylor( void )
{
  double start, stop; /* times of beginning and end of procedure */
  double e, pi, factorial, product;
  int i;

  printf("Parallel Taylor\n");

  /* start the timer */
  start = clock();

  /* Now there is no first and seccond, we calculate e and pi */
#pragma omp parallel sections //shared(e, pi)
  {
#pragma omp section
    {
      printf("e started\n");
      e = 1;
      factorial = 1; /* rather than recalculating the factorial from
			scratch each iteration we keep it in this varialbe
			and multiply it by i each iteration. */
      for (i = 1; i<num_steps; i++) {
	factorial *= i;
	e += 1.0/factorial;
      }
      printf("e done\n");
    } /* e section */

#pragma omp section
    {
      /* In this thread we calculate pi expansion */
      printf("pi started\n");

      pi = 0;
      for (i = 0; i < num_steps*10; i++) {
	/* we want 1/1 - 1/3 + 1/5 - 1/7 etc.
	   therefore we count by fours (0, 4, 8, 12...) and take
             1/(0+1) =  1/1
	   - 1/(0+3) = -1/3
             1/(4+1) =  1/5
	   - 1/(4+3) = -1/7 and so on */
	pi += 1.0/(i*4.0 + 1.0);
	pi -= 1.0/(i*4.0 + 3.0);
      }
      pi = pi * 4.0;
      printf("pi done\n");
    } /* pi section */

  } /* omp sections */
  /* at this point the threads should rejoin */

  product = e * pi;

  stop = clock();

  printf("Reached result %f in %.3f seconds\n", product, (stop-start)/1000);

}

void display_matrix( unsigned char mat[][8] )
{
    int i, j;
    for( i = 0 ; i < 8; i++ )
    {
        for( j = 0 ; j < 8; j++ )
            printf("%3d ", mat[i][j] );
        printf("\n");
    }
    printf("\n");

}

const int PI = 3.141592;

void matrix_multiplication( void )
{

    unsigned __int64 time_stamp_start, time_stamp_duration;

    __declspec(align(16)) unsigned char matA[8][8] = {  { 0, 1, 2, 3, 4, 5, 6, 7 },
                        { 0, 0, 10, 11, 12, 13, 14, 15 },
                        { 0, 0, 0, 3, 4, 5, 6, 7 },
                        { 0, 0, 0, 0, 12, 13, 14, 15 },
                        { 0, 0, 0, 0, 0, 5, 6, 7 },
                        { 0, 0, 0, 0, 0, 0, 14, 15 },
                        { 0, 0, 0, 0, 0, 0, 0, 7 },
                        { 0, 0, 0, 0, 0, 0, 0, 0 }};
    __declspec(align(16)) unsigned char matB[8][8] = {  { 0, 0, 0, 0, 0, 0, 0, 0 },
                        { 0, 0, 0, 0, 0, 0, 0, 7 },
                        { 0, 0, 0, 0, 0, 0, 14, 15 },
                        { 0, 0, 0, 0, 0, 5, 6, 7 },
                        { 0, 0, 0, 0, 12, 13, 14, 15 },
                        { 0, 0, 0, 3, 4, 5, 6, 7 },
                        { 0, 0, 10, 11, 12, 13, 14, 15 },
                        { 0, 1, 2, 3, 4, 5, 6, 7 }};

    __declspec(align(16)) unsigned char matC[8][8];

    memset( matC, 0, 64*sizeof(char) );

    int i, j, k;
    int iteration;
    int nThreads;
    unsigned char temp;
    clock_t start_t, duration_t;

    printf("Single\n");

    INIT_PERFORMANCE_MEASURE();

    START_PERFORMANCE_MEASURE();

    start_t = clock();

    for( iteration = 0; iteration < 90000; iteration++ )
    {
        for( i = 0; i < 8; i++ )
            for( j = 0; j < 8; j++ )
            {
                temp = 0;
                for( k = 0; k < 8; k++ )
                {
                    temp += matA[i][k]*matB[k][j];
                }
                matC[i][j] = temp;
            }
    }
    duration_t = clock() - start_t;

    STOP_PERFORMANCE_MEASURE();

    printf("Duration = %f (%f)\n", (double)duration_t/CLOCKS_PER_SEC,
        GET_PERFORMANCE_MEASURE() );

    display_matrix( matC );
    memset( matC, 0, 64*sizeof(unsigned char) );

    printf("Multithreaded 2\n");

    START_PERFORMANCE_MEASURE();
    start_t = clock();

    #pragma omp parallel default(none) private(i,j,k,temp) shared(nThreads, matA, matB, matC) num_threads(2)
    {
        #pragma omp master
        nThreads = omp_get_num_threads();

        #pragma omp for
        for( iteration = 0; iteration < 90000; iteration++ )
        {
            for( i = 0; i < 8; i++ )
                for( j = 0; j < 8; j++ )
                {
                    temp = 0;
                    for( k = 0; k < 8; k++ )
                    {
                        temp += matA[i][k]*matB[k][j];
                    }
                    matC[i][j] = temp;
                }
        }
   }
    duration_t = clock() - start_t;

    STOP_PERFORMANCE_MEASURE();

    printf("Duration = %f (%f)\n", (double)duration_t/CLOCKS_PER_SEC, GET_PERFORMANCE_MEASURE() );

    display_matrix( matC );
    printf("nThreads : %d\n", nThreads);
    printf("\n");

    /////////////////////////////////////////////////////////////////////////////
    printf("Using SIMD\n");

    __m128i a, b, c, zero, high_temp, low_temp, high_temp2, low_temp2, temp_128, temp2_128;

    start_t = clock();

    zero = _mm_setzero_si128();
    for( iteration = 0; iteration < 90000; iteration++ )
    {
        for( i = 0; i < 8; i++ )
        {
            // ith row
            a = _mm_set_epi16( matA[i][7], matA[i][6],
                              matA[i][5], matA[i][4],
                              matA[i][3], matA[i][2],
                              matA[i][1], matA[i][0] );

            //a = _mm_unpacklo_epi8( a, zero ); // Now they are in 16bits

            for( j = 0; j < 8; j++ )
            {
                b = _mm_set_epi16( matB[7][j], matB[6][j], matB[5][j], matB[4][j],
                               matB[3][j], matB[2][j], matB[1][j], matB[0][j]);

                //b = _mm_unpacklo_epi8( b, zero ); // Now they are in 16bits

                low_temp = _mm_mullo_epi16( a, b );
                high_temp = _mm_mulhi_epi16( a, b );

                low_temp2 = _mm_unpacklo_epi16( low_temp, high_temp ); // 32bits
                high_temp2 = _mm_unpackhi_epi16( low_temp, high_temp ); // 32bits

                temp_128 = _mm_add_epi32( low_temp2, high_temp2 );
                temp2_128 = _mm_shuffle_epi32( temp_128, 0x4E );

                temp_128 = _mm_add_epi32( temp_128, temp2_128 );
                temp2_128 = _mm_shuffle_epi32( temp_128, 0xB1 );

                temp_128 = _mm_add_epi32( temp_128, temp2_128 );

                matC[i][j] = temp_128.m128i_i8[0];

            }
        }
    }
    duration_t = clock() - start_t;
    printf("Duration = %f\n", (double)duration_t/CLOCKS_PER_SEC );

    display_matrix( matC );
    printf("\n");

    /////////////////////////////////////////////////////////////////////////////
    printf("Using SIMD with threads\n");
    start_t = clock();

    start_t = clock();

    zero = _mm_setzero_si128();
#pragma omp parallel default(none) private(i, j, a, b, low_temp, high_temp, low_temp2, high_temp2, temp_128, temp2_12 8) shared( nThreads, matA, matB, matC, zero )  num_threads(2)
{
#pragma omp master
        nThreads = omp_get_num_threads();

#pragma omp for
    for( iteration = 0; iteration < 90000; iteration++ )
    {
        for( i = 0; i < 8; i++ )
        {
            // ith row
            a = _mm_set_epi16( matA[i][7], matA[i][6],
                              matA[i][5], matA[i][4],
                              matA[i][3], matA[i][2],
                              matA[i][1], matA[i][0] );

            //a = _mm_unpacklo_epi8( a, zero ); // Now they are in 16bits

            for( j = 0; j < 8; j++ )
            {
                b = _mm_set_epi16( matB[7][j], matB[6][j], matB[5][j], matB[4][j],
                               matB[3][j], matB[2][j], matB[1][j], matB[0][j]);

                //b = _mm_unpacklo_epi8( b, zero ); // Now they are in 16bits

                low_temp = _mm_mullo_epi16( a, b );
                high_temp = _mm_mulhi_epi16( a, b );

                low_temp2 = _mm_unpacklo_epi16( low_temp, high_temp ); // 32bits
                high_temp2 = _mm_unpackhi_epi16( low_temp, high_temp ); // 32bits

                temp_128 = _mm_add_epi32( low_temp2, high_temp2 );
                temp2_128 = _mm_shuffle_epi32( temp_128, 0x4E );

                temp_128 = _mm_add_epi32( temp_128, temp2_128 );
                temp2_128 = _mm_shuffle_epi32( temp_128, 0xB1 );

                temp_128 = _mm_add_epi32( temp_128, temp2_128 );

                matC[i][j] = temp_128.m128i_i8[0];
            }
        }
    }
}
    duration_t = clock() - start_t;
    printf("Duration = %f\n", (double)duration_t/CLOCKS_PER_SEC );

    display_matrix( matC );
    printf("nThreads : %d\n", nThreads);
    printf("\n");

}

int _tmain(int argc, _TCHAR* argv[])
{
    //test(0);
    //test(3);

    //AnotherTest();
    //printf("\n\n");
    //AnotherTest2();

    //sequential_taylor();
    //parallel_taylor();

    matrix_multiplication();

	return 0;
}
