// ascii.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <locale.h>

int main(int argc, char* argv[])
{
	char *ch = setlocale(LC_ALL, "Serbian");
	for(int i=1; i<0xff; i++) {
		printf("0x%x, '%c',\n", i, i);
	}
	return 0;
}
