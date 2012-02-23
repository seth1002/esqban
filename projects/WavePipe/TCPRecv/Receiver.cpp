// TcpStreamServer.cpp: implementation of the CReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "Receiver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReceiver::CReceiver(int portnum)
{
	m_iPortnum = portnum;

	m_WinSocket = INVALID_SOCKET,  // Window socket
	m_ClientSock = INVALID_SOCKET; // Socket for communicating 

	m_OnReceiveData = NULL;

	Init();
}

CReceiver::~CReceiver()
{
	Close();
}

void CReceiver::SetOnReceiveData(void (*func)(const char*, int)) 
{
	m_OnReceiveData = func;
}

BOOL CReceiver::Init()
{
//	TCHAR szError[100];		// Error message string
	
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
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Allocating socket failed. Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (m_iPortnum);  
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// Associate the local address with WinSocket.
	if (bind (m_WinSocket, (struct sockaddr *) &local_sin, sizeof (local_sin)) == SOCKET_ERROR) 
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Binding socket failed. Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_WinSocket);
		return FALSE;
	}
	
	// Establish a socket to listen for incoming connections.
	if (listen (m_WinSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR) 
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Listening to the client failed. Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
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
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Accepting connection with client failed. Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CReceiver::Close()
{
	// Disable both sending and receiving on ClientSock.
	shutdown (m_ClientSock, 0x02);
	
	// Close ClientSock.
	closesocket (m_ClientSock);
	
//	WSACleanup ();
}

BOOL CReceiver::Start()
{
	DWORD dwThreadId = 0;
//	TCHAR szError[100];		// Error message string
	HANDLE hThread = CreateThread(0, 0, threadProc, this, 0, &dwThreadId);
	if (!hThread)
	{
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, "Can't create threadProc! -- %08X\n", GetLastError());
		//MessageBox (NULL, szError, TEXT("Server"), MB_OK);
		return FALSE;
	}
	CloseHandle(hThread);

	return TRUE;
}

DWORD WINAPI CReceiver::threadProc(LPVOID arg)
{
	int index = 0;			// Integer index
	int iReturn;			// Return value of recv function
	char szServerA[100];	// ASCII string 
//	TCHAR szServerW[100];	// Unicode string
//	TCHAR szError[100];		// Error message string

	CReceiver *pServer = (CReceiver*)arg;

	for (;;)
	{
		// Receive data from the client.
		iReturn = recv (pServer->m_ClientSock, szServerA, sizeof (szServerA), 0);
		
		// Check if there is any data received. If there is, display it.
		if (iReturn == SOCKET_ERROR)
		{
			ErrorHandlerEx(__LINE__, __FILE__);
			//wsprintf (szError, TEXT("No data is received, recv failed. Error: %d"), WSAGetLastError ());
			//MessageBox (NULL, szError, TEXT("Server"), MB_OK);
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
		ErrorHandlerEx(__LINE__, __FILE__);
		//wsprintf (szError, TEXT("Sending data to the client failed. Error: %d"), WSAGetLastError ());
		//MessageBox (NULL, szError, TEXT("Error"), MB_OK);
	}

	return(0);
}

void CReceiver::ErrorHandlerEx( WORD wLine, LPSTR lpszFile )
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
