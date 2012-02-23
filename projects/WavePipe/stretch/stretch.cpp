#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>

#define BUFF_LEN	1024

int main(int argc, char* argv[])
{
	int readBytes = 1;
	short buffer[BUFF_LEN];
	short tmpbuf[BUFF_LEN];

	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	int step = (argc==2?atoi(argv[1]):1);

	if(step<=0)
		return;

	while(readBytes > 0) {
		readBytes = fread( buffer, 1, sizeof(buffer), stdin );

		int samples = readBytes / sizeof(short);

		memset(tmpbuf, 0, sizeof(tmpbuf));

		for(int i=0; i<samples; i+=step)
		{
		}
		
		fwrite(buffer, 1, readBytes, stdout);
	}

	return 0;
}
