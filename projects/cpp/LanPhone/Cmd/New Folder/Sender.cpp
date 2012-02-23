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
		wsprintf (szError, TEXT("Allocating socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out source socket's address information.
	source_sin.sin_family		= AF_INET;
	source_sin.sin_port			= htons (m_iSourcePort);  
	source_sin.sin_addr.s_addr	= htonl (INADDR_ANY);
	
	// Associate the source socket's address with the socket, Sock.
	if (bind (m_Sock, (struct sockaddr FAR *) &source_sin, sizeof (source_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Binding socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}
	
	// Set the Time-to-Live of the multicast.
	if (setsockopt (m_Sock, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *)&iOptVal, sizeof (int)) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("setsockopt failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
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

	// Send a message to the multicasting address.
	int result = sendto(m_Sock, buf, size + 1, 0, (struct sockaddr FAR *) &dest_sin, sizeof (dest_sin));
	if (result == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("sendto failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return 0;
	}
	
	return result;
}
