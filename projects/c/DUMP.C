/* Dump - Dump a file in hex format. -w(pause) -a(ascii only) -f(form feed)
	
	  A habit from my CP/M days forced me to write this
	  quick and dirty DEBUG (or DDT) substitute.  [CJS100688]

 	  Compiled in Borland Turbo C 2.0.

	  This is a public domain program and the author assumes
	  absolutely no liability for the use or consequences.

	  If you make any (worthwhile) changes to this source code
	  I would appreciate an addendum to this comment with the
	  modifications made before redistributing. (maybe you could
	  drop me a line and a copy too?).

	  Christian Stanton
	  c/o Pixley-Richards, Inc.
	  Plymouth Industrial Pk.
	  Plymouth, MA. 02360
	  (508)-746-6082 voice
*/

#include <stdio.h>
#include <ctype.h>
#include <io.h>

#define TRUE  -1
#define FALSE 0

int wait,asconly,formfeed;

main(argc,argv)
char *argv[];
int argc;
{
  int c;
  char *arg;

  wait=TRUE;
  asconly=FALSE;
  formfeed=FALSE;

  if(argc==1)
    usage("Too few parameters.");

  for(c=argc;c>0;c--){
    arg=argv[c];
    if(arg[0]=='-'){
      switch(tolower(arg[1])) {
        case 'w': wait=FALSE;
         	  printf("\nDon't wait for page");
     		  break;
        case 'a': asconly=TRUE;
        	  printf("\nAscii mode");
     		  break;
        case 'f': formfeed=TRUE;
        	  wait=FALSE;
        	  printf("\nFormfeed between files");
     		  break;
        default : usage("Invalid switch");
        	  printf("-%c",arg[1]);
      }
    }
  }

  for(c=1;c<argc;c++){
    arg=argv[c];
    if(arg[0]!='-')
      dump(arg);
  }

}

usage(msg)
char *msg;
{
  printf("\nUsage: dump filename.ext {f2.x...fn.x} {-[waf]}\n");
  printf(  "       wait,ascii only,formfeed\n\n%s\n",msg);
}

dump(file)
char *file;
{
  FILE *fopen(), *fp;
  char x[17],c;
  int s,l,sc,fsz;

/* the non-standard toggle 'b' is used to supress CR-LF>LF translation by Tc*/
/*  see also Reference Manual under fopen() and _fmode in <fcntl.h>         */

  if((fp=fopen(file,"rb"))==NULL){
    printf("\nCannot open %s.",file);
  } else {
    fsz=filelength(fileno(fp));
    printf("\n%s %u (%xh) bytes\n",file,fsz,fsz);
    l=0;
    s=0;
    sc=0;
    while(!feof(fp)){
      c=getc(fp);
      if(asconly){
        if(!(s++%64)){
          if(!(++l%24)&&wait){
            wait=pause();
          }
          printf("%04x: ",s-1);
        }
        printf("%c",(isprint(c))?c:'.');
        if(!(s%64)){
          printf("\n");
        }
      } else {
        x[s++]=c;
        x[s]=0;
        if(s==16){
          sc+=s;
          if(!(++l%24)&&wait){
            wait=pause();
          }
          printf("%04x: ",sc-16);
          for(s=0;s<16;s++){
            printf("%02x ",x[s]&0xff); /* &0xff for annoying Tc hex bug */
            x[s]=(isprint(x[s]))?x[s]:'.';
          }
          printf(":%s:\n",x);
          x[0]=0;
          s=0;
        }
      }
    }
    if(!asconly&&s>0){
      sc+=s;
      if(!(++l%24)&&wait){
        wait=pause();
      }
      printf("%04x: ",sc-16);
      for(l=0;l<16;l++){
        if(l<s-1){
          printf("%02x ",x[l]&0xff);
          x[l]=(isprint(x[l]))?x[l]:'.';
        } else {
          printf("   ");
          x[l]=' ';
        }
      }
      printf(":%s:\n",x);
    }
    fclose(fp);
    if(formfeed)
      printf("\f");
  }
}

int pause()
{ /* returns TRUE if continue, FALSE if no wait, exits to DOS if abandon */
  int c;
  printf("Wait...\r");
  if((c=getch())==3){
    printf("Bye...\n");
    exit();
  }
  if(c=='C'||c=='c')
    return(FALSE);
  else
    return(TRUE);
}

