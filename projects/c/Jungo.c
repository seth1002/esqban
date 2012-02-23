// Jungo.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>

#define BYTE char


void *malloc_aligned(int sz)
{
	/*sz += sizeof(int) + 16;
	void *p0 = malloc(sz);
	void *p1 = p1 + sizeof(int);
	unsigned long p = (unsigned long)p1;
	aligned = p % 16 ? (16-p%16) : 0;
	(int)*(p1 + aligned - sizeof(int)) = aligned;
	return p1 + aligned;*/

	BYTE *p0, *p1;
	unsigned long p;
	int aligned;
	int *n;

	sz += sizeof(int) + 16;
	p0 = (BYTE*)malloc(sz);
	p1 = (BYTE*)p0 + sizeof(int);
	p = (unsigned long)p1;
	aligned = p % 16 ? (16-p%16) : 0;
	n = (int*)(p1 + aligned - sizeof(int));
	*n = aligned;
	return p1 + aligned;
}

void free_aligned(void *p)
{
	//int aligned = (int)*(p - sizeof(int));
	//free(p - sizeof(int) - aligned);

	BYTE *p1 = (BYTE*)p;
	int aligned = (int)*(p1 - sizeof(int));
	free(p1 - sizeof(int) - aligned);
}

int main(int argc, char* argv[])
{
	void *p;
	int aligned;

	p = malloc_aligned(101);
	free_aligned(p);
	aligned = (0 == (unsigned long)p % 16);
	return 0;
}
