/*
    Google Scanner 2006.2
    by [RST/GHC] - http://rst.void.ru
    modified by mescalin
    mescalin_@msn.com - http://mescalin.100free.com
    26/05/06

    > mescalin put "safemode off" (that you can scan sites porn, sex and others)
    and put result count default: 100


    compiled and tested in Fedora 5 and CYGWIN(windows)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>

#define HOST "www.google.com" // Google server
#define PORTD 80 // Google port
#define BUFSIZE 256

char *hexdata(char *data); // char to hex
int printlinks(char *data); // print find links
int getport(char *str, int separator); // Get port after separator
void usage(char *pname); // usage
void about(char *pname); // about
int comparison(char *str1, char *str2); // comparison
void tout(int sig); // Timeout
int createconnect(char *host, int port, int timet); // resolving & creat socket & connect
int nextchpos(char *str, char *symb, int startpos); // Next char position
char *cutstring(char *str, int startpos, int endpos); // copyng part of string
char *gethost(char *str, int separator); // Get host

/** Comparison **/
int comparison(char *str1, char *str2)
{
  int i;

  if(str1 == NULL || str2 == NULL)
    return 0;

  if(strlen(str1) != strlen(str2))
    return 0;

  for(i = 0; i < strlen(str1); i++)
    {
      if(str1[i] != str2[i])
        return 0;
    }

  return 1;
}

/** Next char position **/
int nextchpos(char *str, char *symb, int startpos)
{
  int i;

  // Errors
  if(startpos >= strlen(str)) return -1;
  if(startpos < 1) return -2;

  for(i = startpos; i < strlen(str); i++)
    {
      if(str[i] == symb[0])
        return i;
      if(i == strlen(str)) // Not found
        return -3;
      if(i - startpos > 1024)
        return -4;
    }
}

/** Copyng part of string **/
char *cutstring(char *str, int startpos, int endpos)
{
  int i, k = 0;
  char *tmpstr;

  if( startpos < 0 || endpos <= 0 || startpos > endpos)
    return NULL;

  if( startpos > strlen(str) || endpos > strlen(str))
    return NULL;

  /* Memory for 'tmpstr' */
  if((tmpstr = (char *)malloc(sizeof(char) * ( endpos - startpos + 1))) < 0)
    {
      fprintf(stderr, "error: malloc() failed for 'tmpstr'\n");
      return NULL;
    }

  /* Copy part of string */
  for( i = startpos; i <= endpos; i++)
    {
      tmpstr[k] = str[i];
      k++;
    }

  tmpstr[k] = '\0';
  return tmpstr;
}

/** Query to hex **/
char *hexdata(char *data)
{
  int i = -1;
  char *qq="", *p;

  /* Size for qq ('%' + hex) */
  if(( qq = (char *)malloc(sizeof(char) * strlen(data) * 3 + 1)) < 0)
    {
      fprintf(stderr, "error: malloc() failed for 'qq'\n");
      return NULL;
    }
 
  for(p = data; *p; p++)
    {
      sprintf(qq, "%s%%%X", qq, *p);
      i += 3;
    }

  qq[i+1] = '\0';

  return qq;
}
/*
int createconnect(char *hosta, int porta)
{
  struct sockaddr_in saddr;
  struct hostent *hst;
  struct in_addr rhost;

  int sd;

  if(( hst = gethostbyname(hosta)) > 0)
    memcpy(&rhost.s_addr, hst->h_addr, 4);
  else
    if(( rhost.s_addr = inet_addr(hosta)) == INADDR_NONE)
      {
        fprintf(stderr, "error[%d]: host '%s' not resolved\n", errno, hosta);
        return -1;
      }

  bzero(&saddr, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(porta);
  saddr.sin_addr.s_addr = rhost.s_addr;

  if(( sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr, "error[%d]: can't creat socket\n", errno);
      return -2;
    }

  if( connect(sd, (struct sockaddr*)&saddr, sizeof(struct sockaddr)) < 0)
    {
      close(sd);
      fprintf(stderr, "error[%d]: can't connect to host\n", errno);
      return -3;
    }

  return sd;
}
*/
/** Resolving & Creat socket & Connecting **/
int createconnect(char *host, int port, int timet)
{
  struct sockaddr_in saddr;
  struct hostent *hst;
  struct in_addr rhost;

  int sd;

  // Resolving host
  if(( hst = gethostbyname(host)) > 0)
    memcpy(&rhost.s_addr, hst->h_addr, 4);
  else
    if(( rhost.s_addr = inet_addr(host)) == INADDR_NONE)
      {
        fprintf(stderr, "error: %s not resolved\n", host);
        return -1;
      }

  // Socket struct //
  bzero(&saddr, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = rhost.s_addr;

  // Create socket //
  if(( sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr, "error: can't creat socket\n");
      return -2;
    }

  // Timeout //
  signal(SIGALRM, tout);

  // Set timeout //
  alarm(timet);

  // Connect to host //
  if( connect(sd, (struct sockaddr*)&saddr, sizeof(struct sockaddr)) < 0)
    {
      close(sd);
      fprintf(stderr, "error: can't connect to %s\n", host);
      return -3;
    }

  return sd;
}

/** Timeout **/
void tout(int sig)
{
  fprintf(stderr, "error: timeout\nexit\n");
  exit(100);
}

void about(char *pname)
{
  printf("\nGoogle Scanner 2006.2\n");
  printf("by [RST/GHC] - http://rst.void.ru\n");
  printf("> modified by mescalin\n");
  printf("> mescalin_@msn.com - http://www.mescalin.cjb.net\n\n");
}

void usage(char *pname)
{
  about(pname);
  printf("Usage:\t %s <\"params\">\n", pname);
  printf("\nList of params:\n");
  printf(" -q <\"query\">\t -- search query\n");
  printf(" -s <server>\t -- search server [default: www.google.com]\n");
  printf(" -p <port>\t -- connect port [default: 80]\n");
  printf(" -w <host:port> -- proxy server\n");
  printf(" -c <count>\t -- result count [default: 100]\n");
  printf(" -b <start>\t -- start page [default: 0]\n");
  printf(" -t <timeout>\t -- set timeout (sec) [default: 60]\n");
  printf(" -h\t\t -- print info and exit\n");
  printf("\nExample:\n");
  printf(" %s -q \"inurl:index.php\"\n", pname);
}

/** Get host **/
char *gethost(char *str, int splitter)
{
  int i, c;
  char *p;

  c = 0;

  // Invalid string //
  if(str == NULL)
    return NULL;

  // Invalid splitter //
  if(splitter > 255 || splitter < 0)
    return NULL;

  // Find splitter //
  for(i = 0; i < strlen(str); i++)
    if(str[i] == splitter)
      {
        c = i;
        break;
      }

  // Splitter not found //
  if(c == 0)
    return NULL;

  if(( p = (char*)malloc(sizeof(char) * strlen(str) + 1)) < 0)
    {
      fprintf(stderr, "error: malloc() failed for 'p'\n");
      return NULL;
    }

  // Copy string //
  memcpy(p, str, sizeof(char) * (strlen(str) + 1));

  p[c] = '\0';
  return p;
}

/* Get port */
int getport(char *str, int splitter)
{
  int i, cplt, port;
  char *p;

  cplt = 0;

  // Invalid splitter //
  if(splitter > 255 || splitter < 0)
    return -1;

  // Invalid string //
  if(str == NULL)
    return -2;

  // Find splitter //
  for( i = 0; i < strlen(str); i++)
    if(str[i] == splitter)
      {
        cplt = i;
        break;
      }

  // Splitter not found //
  if(cplt == 0)
    return -3;

  // Invalid port range //
  if(strlen(str) - cplt > 6)
    return -4;

  if((p = (char *)malloc(sizeof(char) * (strlen(str) + 1))) < 0)
    {
      fprintf(stderr, "error: malloc() failed for 'p'\n");
      return -5;
    }

  // Copy string //
  memcpy(p, str, sizeof(char) * (strlen(str) + 1));

  // Move pointer //
  for(i = 0; i < cplt + 1; i++)
    p++;

  port = atoi(p);

  // Invalid port
  if(port > 65535)
    return -6;

  return port;
}

/** Links **/
int printlinks(char *data)
{
  char *p, *result = "";
  int i = -1, j, size = sizeof(char);
  int start, end;

  char *const1 = "<a class=l";

  if(( result = (char *)malloc(sizeof(char))) < 0)
    {
      fprintf(stderr, "error: malloc() failed for 'result'\n");
      return -1;
    }

  for(p = data; *p; p++)
    {
      i++;

      // if const1 found ...
      if( *p == const1[strlen(const1) - 1] && comparison(cutstring(data, i - strlen(const1) + 1, i), const1))
        {
          start = 0;
          end = 0;

          if((start = nextchpos(data, "\"", i)) > 0) // search first '"'
            if((end = nextchpos(data, "\"", start + 1)) > 0) // search last '"'
              printf("%s\n", cutstring(data, start + 1, end - 1));
        }
     }

  return 1;
}

int main(int arg, char **param)
{
  char server[BUFSIZE] = HOST; // default www.google.com
  char *query = ""; // search query
  char c; // options
  char *enquery = ""; // hex query
  char *httpquery = ""; // http query
  char count[BUFSIZE] = "100"; // result count
  char *data; // html reply
  char buffer[BUFSIZE + 1]; // server reply
  char start[BUFSIZE] = "0"; // start page
  char *prxserv= ""; // proxy server + prx port
  char *prxserver; // proxy host
  int timeout = 60; // timeout
  int port = PORTD; // default port 80
  int prxport = 8080; // proxy port
  int sd; // socket descriptor
  int size = 0; // reply count
  ssize_t nread; // bytes count

  /* Arguments */
  while(( c = getopt(arg, param, "p:q:c:s:b:w:t:h")) > 0)
    switch(c)
      {
        case 'p':
          port = atoi(optarg);
          break;
        case 'q':
          if(( query = (char *)malloc(strlen(optarg) * (sizeof(char) + 1))) < 0)
            {
              fprintf(stderr, "error: malloc() failed for 'query'\n");
              exit(1);
            }
          strncpy(query, optarg, BUFSIZE);
          break;
        case 'c':
          strncpy(count, optarg, BUFSIZE);
          break;
        case 's':
          strncpy(server, optarg, BUFSIZE);
          break;
        case 'b':
          strncpy(start, optarg, BUFSIZE);
          break;
        case 'w':
          if(( prxserv = (char *)malloc(strlen(optarg) * (sizeof(char) + 1))) < 0)
            {
              fprintf(stderr, "error: malloc() failed for 'prxserv'\n");
              exit(1);
            }
          strncpy(prxserv, optarg, BUFSIZE);
          // length test
          if( strlen(prxserv) > 254 )
            {
              fprintf(stderr, "error: too long value of variable \n");
              exit(1);
            }
          break;
        case 't':
          timeout = atoi(optarg);
          break;
        case 'h':
          usage(param[0]);
          exit(0);
          break;
        default:
          usage(param[0]);
          exit(0);
          break;
      }

  /* Default params */
  // port check
  if( port > 65535 | port < 1)
    {
      fprintf(stderr, "error: ports must be > 0 and < 65535\n");
      exit(2);
    }

  // count limit ;)
  if(atoi(count) < 1)
    {
      fprintf(stderr, "error: count must be > 0\n");
      exit(3);
    }

  // start page
  if(atoi(start) < 0)
    {
      fprintf(stderr, "error: start page must be > 0\n");
      exit(8);
    }

  // no query
  if(comparison(query, ""))
    {
      usage(param[0]);
      exit(0);
    }

  // encoding
  if(( enquery = hexdata(query)) == NULL)
    exit(4);

  // if not proxy ...
  if(strlen(prxserv) < 1)
    {
      // hex data + http query + result count + start
      if(( httpquery = (char *)malloc(sizeof(char) * (strlen(count) + strlen(start) + strlen(enquery) + 39))) < 0)
        {
          fprintf(stderr, "error: malloc() failed for 'httpquery'\n");
          exit(5);
        }
  
      /* Make http query */
      sprintf(httpquery, "GET /search?q=%s&num=%s&start=%s HTTP/1.0\n\n", enquery, count, start);

      // Resolving & Creat socket & Connecting
      if((sd = createconnect(server, port, timeout)) < 0)
        {
          fprintf(stderr, "error: can't connect to %s\n", server);
          exit(29);
        }
    }
  else // if proxy
    {
      // get proxy host
      if((prxserver = gethost(prxserv, ':')) == NULL)
        {
          fprintf(stderr, "error: invalid proxy hostname\n");
          exit(30);
        }
 
      // get proxy port
      if((prxport = getport(prxserv, ':')) < 0)
        {
          fprintf(stderr, "error: invalid proxy port number\n");
          exit(31);
        }

      // hex data + http query + result count + start
      if(( httpquery = (char *)malloc(sizeof(char) * (strlen(server) + strlen(count) + strlen(start) + strlen(enquery) + 53))) < 0)
        {
          fprintf(stderr, "error: malloc() failed for 'httpquery'\n");
          exit(5);
        }

      /* Make http query */
      sprintf(httpquery, "GET http://%s:%d/search?q=%s&num=%s&start=%s HTTP/1.0\n\n", server, port, enquery, count, start);

      // Resolving & Creat socket & Connecting
      if((sd = createconnect(prxserver, prxport, timeout)) < 0)
        {
          fprintf(stderr, "error: connect to proxy '%s:%d' failed\n", prxserver, prxport);
          exit(40);
        }
    }

  if(( data = (char *)malloc(sizeof(char))) < 0)
    {
      fprintf(stderr, "Error: malloc Failed for 'rbuffer'\n");
      exit(6);
    }
  
  /* Send http query */
  write(sd, httpquery, strlen(httpquery));

  /* Read data */
  while(( nread = read(sd, buffer, BUFSIZE - 1)) > 0) // Read data
    {
      size += strlen((char *)buffer) + 1;

      if(( data = (char *)realloc(data, size + strlen((char *)buffer))) < 0)
        {
          close(sd);
          fprintf(stderr, "Error: realloc Failed for 'data'\n");
          exit(7);
        }

      sprintf(data, "%s%s", data, buffer);
      bzero(buffer, BUFSIZE); // Clear memory
    }

  // print links
  printlinks(data);
  exit(0);
}
