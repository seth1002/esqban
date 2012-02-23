// Sender.cpp: implementation of the CSender class.
//
//////////////////////////////////////////////////////////////////////

#include "Sender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSender::CSender(int iSrcPort, int iDestPort, char *pszIP)
{
	m_Sock = INVALID_SOCKET;
	
	m_iSourcePort = 0;
	m_iDestPort = 2000;
	
	strnset(m_szIPAddress, 0, 15);
	strncpy(m_szIPAddress, pszIP, strlen(pszIP));

	Init();
}

CSender::~CSender()
{
	Close();
}

BOOL CSender::Init()
{
	int iOptVal = 64;                   
	TCHAR szError[100] = {0};	// Error message string
	
	// Create a datagram window socket, m_Sock.
	if ((m_Sock = socket (AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Allocating socket failed! Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out source socket's address information.
	source_sin.sin_family		= AF_INET;
	source_sin.sin_port			= htons (m_iSourcePort);  
	source_sin.sin_addr.s_addr	= htonl (INADDR_ANY);
	
	// Associate the source socket's address with the socket, Sock.
	if (bind (m_Sock, (struct sockaddr FAR *) &source_sin, sizeof (source_sin)) == SOCKET_ERROR) 
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Binding socket failed! Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}
	
	// Set the Time-to-Live of the multicast.
	if (setsockopt (m_Sock, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *)&iOptVal, sizeof (int)) == SOCKET_ERROR)
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("setsockopt failed! Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}
	
	// Fill out the desination socket's address information.
	dest_sin.sin_family			= AF_INET;
	dest_sin.sin_port			= htons (m_iDestPort);  
	dest_sin.sin_addr.s_addr	= inet_addr (m_szIPAddress);

	return TRUE;
}

void CSender::Close()
{
	// Disable sending on Sock before closing it.
	shutdown (m_Sock, 0x01);
	
	// Close m_Sock.
	closesocket (m_Sock);
}

int CSender::Send(const char *buf, int size)
{
	TCHAR szError[100]={0};			// Error message string

	// The sendto function is used to write outgoing data on a socket.
	// For message-oriented sockets, care must be taken not to exceed
	// the maximum packet size of the underlying subnets, which can be 
	// obtained by using getsockopt to retrieve the value of socket 
	// option SO_MAX_MSG_SIZE. If the data is too long to pass atomically
	// through the underlying protocol, the error WSAEMSGSIZE is returned 
	// and no data is transmitted.

	//char val[100];
	//int r = getsockopt(m_Sock, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)optval, 100);


	// Send a message to the multicasting address.
	int result = sendto(m_Sock, buf, size+1, 0, 
		(struct sockaddr FAR *)&dest_sin, sizeof(dest_sin));
	if (result == SOCKET_ERROR)
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("sendto failed! Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return 0;
	}
	
	return result;
}

void CSender::ErrorHandlerEx( WORD wLine, LPSTR lpszFile )
{
	LPVOID lpvMessage;
	DWORD  dwError;
	char szBuffer[256];
	
	// Allow FormatMessage() to look up the error code returned by GetLastError
	dwError = FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
								FORMAT_MESSAGE_FROM_SYSTEM, 
								NULL, 
								WSAGetLastError(), 
								MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
								(LPTSTR)&lpvMessage, 
								0, 
								NULL );
	
	// Check to see if an error occured calling FormatMessage()
	if (0 == dwError)
	{
		wsprintf(   szBuffer, 
					"An error occured calling FormatMessage()."
					"Error Code %d", 
					GetLastError() );
		MessageBox( NULL, 
					szBuffer, 
					"Generic", 
					MB_ICONSTOP | MB_ICONEXCLAMATION );
		return;
	}
	
	// Display the error infromation along with the place the error happened.
	wsprintf(szBuffer, "Generic, Line=%d, File=%s", wLine, lpszFile);
	MessageBox(NULL, (char*)lpvMessage, szBuffer, MB_ICONEXCLAMATION | MB_OK);
}
