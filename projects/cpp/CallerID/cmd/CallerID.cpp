// CallerID.cpp : Defines the entry point for the console application.
//

#include "SerialPort\SerialPort.h"
#include <stdio.h>

#define LOG_FILE_NAME		"CALLERID.TXT"
#define NUMBERS_FILE_NAME	"NUMBERS.TXT"
#define	COM_PORT			3
#define	BAUD_RATE			115200
#define	INIT_STR			"AT +VCID=1\0"

FILE *fpLog		= NULL;
char szBuff[100]={0};
int iSize		= 0;

char szDate[10]	= {0};
char szTime[10]	= {0};
char szNmbr[20]	= {0};

char szDay[3]	= {0};
char szMonth[3]	= {0};

char szHour[3]	= {0};
char szMin[3]	= {0};

struct PhoneEntry
{
	char szName[100], szNumber[100];
	PhoneEntry *next;
} phoneEntry;



PhoneEntry *createPE(char *pszName, char *pszNumber)
{
	PhoneEntry *pe = NULL;
	if(0 == strlen(phoneEntry.szNumber)) {
		pe = &phoneEntry;
	} else {
		pe = (PhoneEntry*)malloc(sizeof(PhoneEntry));
		PhoneEntry *tmp = &phoneEntry;
		while(tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = pe;
	}
	pe->next = NULL;
	memset(pe->szName, 0, sizeof(pe->szName));
	memset(pe->szNumber, 0, sizeof(pe->szNumber));
	strcpy(pe->szName, pszName);
	strcpy(pe->szNumber, pszNumber);
	return pe;
}

char *getName(char *pszNumber)
{
	PhoneEntry *tmp = &phoneEntry;
	while(tmp != NULL) {
		if(0==strcmp(tmp->szNumber, pszNumber))
			return tmp->szName;
		tmp = tmp->next;
	}
	return NULL;
}

void load()
{
	char szFilename[] = NUMBERS_FILE_NAME;
	printf("Loading numbers from '%s': ", szFilename);
	FILE *fp = fopen(szFilename, "r");
	char szName[100]={0}, szNumber[100]={0};
	if(NULL != fp) {
		printf("OK\n");
		while(EOF != fscanf(fp, "%s\t", szNumber)) {
			fgets(szName, sizeof(szName)/sizeof(char), fp);
			PhoneEntry *pe = createPE(szName, szNumber);
			memset(szName, 0, sizeof(szName));
			memset(szNumber, 0, sizeof(szNumber));
		}
		fclose(fp);
	} else {
		printf("not found.\n");
	}
}

void clean()
{
	PhoneEntry *tmp = phoneEntry.next;
	PhoneEntry *prev;
	while(tmp != NULL) {
		prev = tmp;
		tmp = tmp->next;
		free(prev);
	}
}

void OnReceiveChar(const char ch)
{
	if(ch!=13) {
		if(ch!=10 && iSize<=sizeof(szBuff))
			szBuff[iSize++] = ch;
	}
	else {
		if(iSize > 7) {
			if(0 == strncmp(szBuff, "DATE", 4)) {
				memset(szDate, 0, sizeof(szDate));
				strncpy(szDate, szBuff + 7, strlen(szBuff + 7));
				if(strlen(szDate) >= 4) {
					szDay[0] = szDate[0];
					szDay[1] = szDate[1];

					szMonth[0] = szDate[2];
					szMonth[1] = szDate[3];
				}
			}
			else if(0 == strncmp(szBuff, "TIME", 4)) {
				memset(szTime, 0, sizeof(szTime));
				strncpy(szTime, szBuff + 7, strlen(szBuff + 7));
				if(strlen(szTime) >= 4) {
					szHour[0] = szTime[0];
					szHour[1] = szTime[1];

					szMin[0] = szTime[2];
					szMin[1] = szTime[3];
				}
			}
			else if(0 == strncmp(szBuff, "NMBR", 4)) {
				memset(szNmbr, 0, sizeof(szNmbr));
				strncpy(szNmbr, szBuff + 7, strlen(szBuff + 7));

				//printf("%s\t%s\t%s\n", szDate, szTime, szNmbr);
				char msg[100] = {0};
				char cap[100] = "CallerID\0";

				char *pszName = getName(szNmbr);
				if(NULL != pszName) {
					sprintf(msg, "%s/%s\t%s:%s\t%s\t%s", szDay, szMonth, szHour, szMin, szNmbr, pszName);
					memset(cap, 0, sizeof(cap));
					sprintf(cap, "%s - CallerID", pszName);
				} else
					sprintf(msg, "%s/%s\t%s:%s\t%s", szDay, szMonth, szHour, szMin, szNmbr);

				fprintf(stdout, "%s\n", msg);
				fprintf(fpLog,  "%s\n", msg);

				fflush( fpLog );

				MessageBox(NULL, msg, cap, MB_OK | MB_ICONINFORMATION);
			}
		}
		memset(szBuff, 0, sizeof(szBuff));
		iSize = 0;
	}

	//int n = putc(ch, fpLog);
	//n = fflush( fpLog );
	//printf("%c", ch);
}

void OnReceiveStatus(int status)
{
	switch(status)
	{
	case EV_CTS:
		break;
	case EV_RXFLAG:
		break;
	case EV_BREAK:
		break;
	case EV_ERR:
		break;
	case EV_RING:
		break;
	}
}

int main(int argc, char* argv[])
{
	CSerialPort serialPort;

	printf("CallerID, Esqban S. Simon, 2004.\n\n");

	if((argc >= 2) && (0 == strcmp(argv[1], "/?"))) {
		printf("Usage: CallerID [port=3] [baudrate=115200] [CallerID init string=\"AT +VCID=1\"]\n");
		return 0;
	}

	int port = COM_PORT;
	if(argc >= 2)
		port = atoi(argv[1]);

	int baud = BAUD_RATE;
	if(argc >= 3)
		baud = atoi(argv[2]);

	char *strinit = INIT_STR;
	if(argc >= 4)
		strinit = argv[3];

	serialPort.SetOnReceiveChar( OnReceiveChar );
	serialPort.SetOnReceiveStatus( OnReceiveStatus );
	
	memset(szBuff, 0, sizeof(szBuff));

	printf("Opening COM%d, %d baud: ", port, baud);
	if(serialPort.InitPort(port, baud)) {
		printf("OK\n");

		char init[100];
		memset(&init, 0, sizeof(init));
		strcpy(init, strinit);
		init[strlen(init)] = 13;
		printf("Activating CallerID: ");
		serialPort.SendData( init, strlen(init) );
		printf("OK\n");

		load();

		printf("Listening on COM%d: ", port);
		if(serialPort.StartMonitoring()) {
			printf("OK\n");

			printf("Opening log file (%s): ", LOG_FILE_NAME);
			fpLog = fopen( LOG_FILE_NAME, "a+");
			if(fpLog) {
				printf("OK\n");

				printf("\nPress ENTER to exit...\n\n");
				printf("Date\tTime\tNumber\t\tName\n");
				printf("-------------------------------------\n");
				getchar();
				//while(true){}

				printf("Closing log.txt: ");
				fclose( fpLog );
				printf("OK\n");
			}
			else // fpLog
				printf("failed!\n");
		}
		else // StartMonitoring
			printf("failed!\n");

		clean();
	}
	else // InitPort
		printf("failed!\n");
	
	return 0;
}

