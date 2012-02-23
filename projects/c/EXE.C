/*
     Program to list header of EXE files

     Written by David Ashley  970102
             dash@netcom.com
*/

struct exe
{
	short signature;
	short lastpagebytes;
	short numpages;
	short numrelocitems;
	short headersize;
	short minalloc;
	short maxalloc;
	short initialss;
	short initialsp;
	short checksum;
	short initialip;
	short initialcs;
	short relocoffset;
	short overlaynum;
};
#define HEADERSIZE 16384
char header[HEADERSIZE];

main(argc,argv)
int argc;
char *argv[];
{
int infile;
struct exe *anexe;
int i,j;
unsigned short *lp;
char name[128];
long progbase;
short val;
long longoff;

	if(argc<2)
	{
		printf("Need EXE filename as parameter\n");
		exit();
	}
	strcpy(name,argv[1]);
	infile=fopen(name,"rb");
	if(!infile) strcat(name,".exe");
	infile=fopen(name,"rb");
	if(!infile)
	{
		printf("Cannot open %s\n",argv[1]);
		exit();
	}
	read(infile,header,HEADERSIZE);
	anexe=header;
	printf("%04x Signature\n",anexe->signature);
	printf("%04x Number of bytes in last page\n",anexe->lastpagebytes);
	printf("%04x Number of 512-byte pages (including last page)\n",anexe->numpages);
	printf("%04x Number of relocation items in the relocation table\n",anexe->numrelocitems);
	printf("%04x Size of header, in paragraphs\n",anexe->headersize);
	printf("%04x Minimum allocation, in paragraphs\n",anexe->minalloc);
	printf("%04x Maximum allocation, in paragraphs\n",anexe->maxalloc);
	printf("%04x Initial SS value (SS=this + 10h + DS)\n",anexe->initialss);
	printf("%04x Initial SP value\n",anexe->initialsp);
	printf("%04x Checksum (ignored by DOS and usually not set)\n",anexe->checksum);
	printf("%04x Initial IP value\n",anexe->initialip);
	printf("%04x Initial CS value (CS=this + 10h + DS)\n",anexe->initialcs);
	printf("%04x Byte offset to relocation table\n",anexe->relocoffset);
	printf("%04x Overlay number\n",anexe->overlaynum);
	progbase=anexe->headersize<<4;
	lp=header+anexe->relocoffset;
	i=anexe->numrelocitems;
/*
	if(i)
	{
		printf("Relocation items: each of the form xxxx:yyyy=zzzz\n");
		printf("EXE file after header gets loaded to DS+10h:0\n");
		printf("At each word pointed to by DS+10h+xxxx:yyyy, add DS+10h.\n");
		printf("Original value of word is zzzz.\n");
	}
*/
	j=0;
	while(i--)
	{
		longoff=*lp+((long)lp[1]<<4L);
		lseek(infile,longoff+progbase,0);
		read(infile,&val,2);
		printf("%04x:%04x=%04x",lp[1],*lp,val);
		lp+=2;
		j++;
		if(i && j<4) putchar(' '); else putchar('\n');
		if(j==4) j=0;
	}
	fclose(infile);
}
