// TcpstreamServer.cpp: implementation of the CTcpstreamServer class.
//
//////////////////////////////////////////////////////////////////////

#include "TcpstreamServer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpstreamServer::CTcpstreamServer(int portnum)
{
	m_iPortnum = portnum;

	m_WinSocket = INVALID_SOCKET,  // Window socket
	m_ClientSock = INVALID_SOCKET; // Socket for communicating 

	m_OnReceiveData = NULL;

	Init();
}

CTcpstreamServer::~CTcpstreamServer()
{
	Close();
}

void CTcpstreamServer::SetOnReceiveData(void (*func)(const char*, int)) 
{
	m_OnReceiveData = func;
}

BOOL CTcpstreamServer::Init()
{
	TCHAR szError[100];		// Error message string
	
	// between the server and client
	SOCKADDR_IN local_sin,	// Local socket address
				accept_sin;	// Receives the address of the connecting entity
	int accept_sin_len;		// Length of accept_sin
	
	/*// Initialize Winsock.
	WSADATA WSAData;		// Contains details of the Winsock implementation
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0) 
	{
		wsprintf (szError, TEXT("WSAStartup failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}*/

	// Create a TCP/IP socket, WinSocket.
	if ((m_WinSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		wsprintf (szError, TEXT("Allocating socket failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (m_iPortnum);  
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// Associate the local address with WinSocket.
	if (bind (m_WinSocket, (struct sockaddr *) &local_sin, sizeof (local_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Binding socket failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_WinSocket);
		return FALSE;
	}
	
	// Establish a socket to listen for incoming connections.
	if (listen (m_WinSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Listening to the client failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_WinSocket);
		return FALSE;
	}
	
	accept_sin_len = sizeof (accept_sin);
	
	// Accept an incoming connection attempt on WinSocket.
	m_ClientSock = accept (m_WinSocket, (struct sockaddr *) &accept_sin, (int *) &accept_sin_len);
	
	// Stop listening for connections from clients.
	closesocket (m_WinSocket);
	
	if (m_ClientSock == INVALID_SOCKET) 
	{
		wsprintf (szError, TEXT("Accepting connection with client failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CTcpstreamServer::Close()
{
	// Disable both sending and receiving on ClientSock.
	shutdown (m_ClientSock, 0x02);
	
	// Close ClientSock.
	closesocket (m_ClientSock);
	
//	WSACleanup ();
}

BOOL CTcpstreamServer::Start()
{
	DWORD dwThreadId = 0;
	TCHAR szError[100];		// Error message string
	HANDLE hThread = CreateThread(0, 0, threadProc, this, 0, &dwThreadId);
	if (!hThread)
	{
		wsprintf (szError, "Can't create threadProc! -- %08X\n", GetLastError());
		MessageBox (NULL, szError, TEXT("Server"), MB_OK);
		return FALSE;
	}
	CloseHandle(hThread);

	return TRUE;
}

DWORD WINAPI CTcpstreamServer::threadProc(LPVOID arg)
{
	int index = 0;			// Integer index
	int iReturn;			// Return value of recv function
	char szServerA[100];	// ASCII string 
//	TCHAR szServerW[100];	// Unicode string
	TCHAR szError[100];		// Error message string

	CTcpstreamServer *pServer = (CTcpstreamServer*)arg;

	for (;;)
	{
		// Receive data from the client.
		iReturn = recv (pServer->m_ClientSock, szServerA, sizeof (szServerA), 0);
		
		// Check if there is any data received. If there is, display it.
		if (iReturn == SOCKET_ERROR)
		{
			wsprintf (szError, TEXT("No data is received, recv failed. Error: %d"), WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Server"), MB_OK);
			break;
		}
		else if (iReturn == 0)
		{
			MessageBox (NULL, TEXT("Finished receiving data"), TEXT("Server"), MB_OK);
			break;
		}
		else
		{
			/*// Convert the ASCII string to the Unicode string.
			for (index = 0; index <= sizeof (szServerA); index++)
				szServerW[index] = szServerA[index];
			
			// Display the string received from the client.
			MessageBox (NULL, szServerW, TEXT("Received From Client"), MB_OK);*/
			
			if(pServer->m_OnReceiveData)
				pServer->m_OnReceiveData(szServerA, strlen(szServerA));
		}
	} 
	
	// Send a string from the server to the client.
	if (send (pServer->m_ClientSock, "To Client.", strlen ("To Client.") + 1, 0) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Sending data to the client failed. Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
	}

	return(0);
}
