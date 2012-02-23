// Receiver.cpp: implementation of the CReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "Receiver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReceiver::CReceiver(int iSrcPort, int iDestPort, char *pszIP)
{
	m_Sock = INVALID_SOCKET;
	
	m_iDestPort = 2000;
	
	strnset(m_szIPAddress, 0, 15);
	strncpy(m_szIPAddress, pszIP, strlen(pszIP));

	m_OnReceiveData = NULL;

	Init();
}

CReceiver::~CReceiver()
{
	Close();
}

BOOL CReceiver::Init()
{
	int index = 0;					// Integer index
	char szMessage[100]={0};		// ASCII string 
	TCHAR szError[100]={0};			// Error message string
	
	struct ip_mreq mreq;			// Used in adding or dropping 
									// multicasting addresses

	// Create a datagram socket, Sock.
	if ((m_Sock = socket (AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
	{
		wsprintf (szError, TEXT("Allocating socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (m_iDestPort);  
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// Associate the local address with Sock.
	if (bind (m_Sock, (struct sockaddr FAR *) &local_sin, sizeof (local_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Binding socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}
	
	// Join the multicast group from which to receive datagrams.
	mreq.imr_multiaddr.s_addr = inet_addr (m_szIPAddress);
	mreq.imr_interface.s_addr = INADDR_ANY;
	
	if (setsockopt (m_Sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&mreq, sizeof (mreq)) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("setsockopt failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}

	return TRUE;
}

void CReceiver::Close()
{
	// Disable receiving on Sock before closing it.
	shutdown (m_Sock, 0x00);
	
	// Close Sock.
	closesocket (m_Sock);
}

DWORD WINAPI CReceiver::threadProc(LPVOID arg)
{
	int iRecvLen;				// Length of recv_sin
	char buff[100] = {0};

	CReceiver *pReceiver = (CReceiver*)arg;

	iRecvLen = sizeof (pReceiver->recv_sin);
	
	while(0 != strncmp(buff, "exit", 4))
	{
		// Receive data from the multicasting group server.
		if (recvfrom (pReceiver->m_Sock, buff, 100, 0, (struct sockaddr FAR *) &pReceiver->recv_sin, &iRecvLen) == SOCKET_ERROR)
		{
			TCHAR szError[100] = {0};	// Error message string
			wsprintf (szError, TEXT("recvfrom failed! Error: %d"), WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Error"), MB_OK);
			closesocket (pReceiver->m_Sock);
			return(0);
		}
		else
		{
			if(pReceiver->m_OnReceiveData)
				pReceiver->m_OnReceiveData(buff, strlen(buff));
			//printf("%s\n", szMessage);
		}
	}

	return(0);
}

BOOL CReceiver::Start()
{
	DWORD dwThreadId = 0;
	HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadProc, this, 0, &dwThreadId);
	if (!hThread)
	{
		//fprintf(stderr, "Can't create threadProc! -- %08X\n", GetLastError());
		return FALSE;
	}
	CloseHandle(hThread);

	return TRUE;
}

void CReceiver::SetOnReceiveData(void (*func)(const char*, int)) 
{
	m_OnReceiveData = func;
}
