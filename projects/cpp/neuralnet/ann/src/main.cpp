
#ifdef WINDOWS
#include <conio.h>
//#else
//#include <curses.h>
#endif
#include "neuralnet.h"

// defining a net with 4 layers having 3,3,3, and 1 neuron respectively,
// the first layer is input layer i.e. simply holder for the input parameters
// and has to be the same size as the no of input parameters, in out example 3
int numLayers = 4, lSz[4] = {3,3,2,1};


// Learing rate - beta
// momentum - alpha
// Threshhold - thresh (value of target mse, training stops once it is achieved)
double beta = 0.3, alpha = 0.1, Thresh =  0.000001;


// maximum no of iterations during training
long num_iter = 6000000;

CSerializableNet net(numLayers, lSz, beta, alpha);

void Train()
{
	// prepare XOR traing data
	double data[][4]={
				{0,	0,	0,	0},
				{0,	0,	1,	1},
				{0,	1,	0,	1},
				{0,	1,	1,	0},
				{1,	0,	0,	1},
				{1,	0,	1,	0},
				{1,	1,	0,	0},
				{1,	1,	1,	1} };

	double error;
	
	printf("\nLearning....\n\n");	
	printf("Iterations\tError\n------------------------\n");
	long i=0;
	for (i=0; i<num_iter ; i++)
	{
		error = net.BackPropagate(data[i%8], &data[i%8][3]);

		if ( i%(num_iter/10) == 0 )
		{
			printf("%ld\t\t%f\n", i, error);
		}
		if(error < Thresh)
		{
			printf("Network trained.\n");
			break;
		}
	}
	
	if ( i == num_iter )
	{
		printf("\n%ld iterations completed.\n", i);  	
	}
}

bool Test()
{
	// prepare test data	
	double testData[][4]={
				{0,	0,	0,	(0^0^0)},
				{0,	0,	1,	(0^0^1)},
				{0,	1,	0,	(0^1^0)},
				{0,	1,	1,	(0^1^1)},
				{1,	0,	0,	(1^0^0)},
				{1,	0,	1,	(1^0^1)},
				{1,	1,	0,	(1^1^0)},
				{1,	1,	1,	(1^1^1)} };

	bool success = true;
	
	printf("\nTesting....\n\n");
	for(int i=0; i<8; i++)
	{
		net.FeedForward( testData[i] );

		int result = net.GetOutput(0) > 0.8;
		success = result == testData[i][3];
		const char *pszResult = success ? "" : "FAILED!";

		printf("%d xor %d xor %d  =  %d  (%f)  %s\n",
			(int)testData[i][0],
			(int)testData[i][1],
			(int)testData[i][2],
			result,
			net.GetOutput(0),
			pszResult);
	}
	if(!success)
		printf("Network not trained well!\n");
	return success;
}

void Save(const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if(fp)
	{
		bool result = net.Write(fp);
		fflush(fp);
		fclose(fp);
		if( result )
			printf("Saved net to '%s'.\n", filename);
		else
			printf("Failed to save net to '%s'!\n", filename);
	}
}

void Load(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if(fp)
	{
		if( net.Read(fp) )
			printf("Loaded net from '%s'.\n", filename);
		else
			printf("Failed to load net from '%s'!\n", filename);
		fclose(fp);
	}
}

void Dump()
{
	FILE *fp = fopen("xor.xml", "w");
	if(fp)
	{
		net.DumpXML(fp);
		fflush(fp);
		fclose(fp);
	}
}

#ifndef WINDOWS
#include <termios.h>
int getch( )
{
	struct termios oldt,
	newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}
#endif

int main(int argc, char **argv)
{
	const char *filename = "xor.nn";
	if(0)
	{
		Load(filename);
		Test();
	}
	else
	{
		Train();
		if( Test() )
			Save(filename);
	}

	Dump();

	printf("\nPres any key...");
	getch();

	return 0;
}
