
#include <stdio.h>
#include <string.h>

void test1()
{
	int x=1;
	int y=1;
	int a = 3*(y-8)/9;
	int b = (y-8)/9*3;
	printf( "a=%d b=%d" , a, b);
}  


void test2()
{
	int i =5;
	i= (++i)/(i++);
	printf( "%d" , i);
}




void main()
{
	printbits(2);
}  



// 1. Write a "Hello World" program in 'C' without using a semicolon.
void main0()
{
	if (printf("Hello World!\n") {}
}


// 2. Write a C++ program without using any loop (if, for, while etc) to print numbers from 1 to 100 and 100 to 1;


// 3. Exchange two numbers without using a temporary variable.
void testXOR()
{
	int x=1;
	int y=2;
	x^=y; y^=x; x^=y;
}

// 4. Find if the given number is a power of 2.
int is_power_of_2( int x )
{
    return (x != 0) && ( ( (x-1) & x ) == 0 );
}

// 5. Multiply x by 7 without using multiplication (*) operator.
int multiply_by_7(int x)
{
	return (x << 3) - x;
}

// 6. Write a function in different ways that will return f(7) = 4 and f(4) = 7



// 10. Convert (integer) number in binary without loops.
void printbits(int x)
{
	int n=x%2;
	if(x>=2)
		printbits(x/2);
	printf("%d", n);
}







