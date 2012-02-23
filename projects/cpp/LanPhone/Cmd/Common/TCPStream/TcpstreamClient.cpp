// TcpStreamClient.cpp: implementation of the CTcpStreamClient class.
//
//////////////////////////////////////////////////////////////////////

#include "TcpStreamClient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpStreamClient::CTcpStreamClient(char *hostname, int portnum)
{
	m_iPortnum = portnum;

	strnset(m_pszHostname, 0, 15);
	strncpy(m_pszHostname, hostname, strlen(hostname));

	m_ServerSock = INVALID_SOCKET;

	Init();
}

CTcpStreamClient::~CTcpStreamClient()
{
	Close();
}










BOOL CTcpStreamClient::Init()
{
	TCHAR szError[100];				// Error message string
	SOCKADDR_IN destination_sin;	// Server socket address
	PHOSTENT phostent = NULL;		// Points to the HOSTENT structure of the server
	WSADATA WSAData;				// Contains details of the Winsocket implementation
	
	// Initialize Winsocket. 
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0) 
	{
		wsprintf (szError, TEXT("WSAStartup failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Create a TCP/IP socket that is bound to the server.
	if ((m_ServerSock = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		wsprintf (szError, TEXT("Allocating socket failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out the server socket's address information.
	destination_sin.sin_family = AF_INET;
	
	// Retrieve the host information corresponding to the host name.
	if ((phostent = gethostbyname (m_pszHostname)) == NULL) 
	{
		wsprintf (szError, TEXT("Unable to get the host name. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_ServerSock);
		return FALSE;
	}
	
	// Assign the socket IP address.
	memcpy ((char FAR *)&(destination_sin.sin_addr), phostent->h_addr, phostent->h_length);
	
	// Convert to network ordering.
	destination_sin.sin_port = htons (PORTNUM);      
	
	// Establish a connection to the server socket.
	if (connect (m_ServerSock, (PSOCKADDR) &destination_sin, sizeof(destination_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Connecting to the server failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_ServerSock);
		return FALSE;
	}

	return TRUE;
}

void CTcpStreamClient::Close()
{
	// Disable receiving on ServerSock.
	shutdown (m_ServerSock, 0x00);
	
	// Close the socket.
	closesocket (m_ServerSock);
	
//	WSACleanup ();
}

int CTcpStreamClient::Send(const char *buf, int size)
{
	int index = 0,			// Integer index
		iReturn;			// Return value of recv function
	char szClientA[100];	// ASCII string 
	TCHAR szClientW[100];	// Unicode string
	TCHAR szError[100];	// Error message string

	// Send a string to the server.
	if (send (m_ServerSock, "To Server.", strlen ("To Server.") + 1, 0) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Sending data to the server failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
	}
	
	// Disable sending on ServerSock.
	shutdown (m_ServerSock, 0x01);

	for (;;)
	{
		// Receive data from the server socket.
		iReturn = recv (m_ServerSock, szClientA, sizeof (szClientA), 0);
		
		// Check if there is any data received. If there is, display it.
		if (iReturn == SOCKET_ERROR)
		{
			wsprintf (szError, TEXT("No data is received, recv failed. Error: %d"), WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Client"), MB_OK);
			break;
		}
		else if (iReturn == 0)
		{
			MessageBox (NULL, TEXT("Finished receiving data"), TEXT("Client"), MB_OK);
			break;
		}
		else
		{
			// Convert the ASCII string to a Unicode string.
			for (index = 0; index <= sizeof (szClientA); index++)
				szClientW[index] = szClientA[index];
			
			// Display the string received from the server.
			MessageBox (NULL, szClientW, TEXT("Received From Server"), MB_OK);
		}
	}

	return 0;
}
