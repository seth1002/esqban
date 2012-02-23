#include <stdio.h>

int main(int argc, char *argv[])
{
	int maxlen = 5;
	int n=0;
	int i=0;
	char c;

	for(n=0; n<maxlen; n++) {
		for(i=0; i<n; i++) {
			for(c='A'; c<'Z'; c++) {
				printf("%c", c);
			}
		}
		printf("\n");
	}
	return 0;
}
