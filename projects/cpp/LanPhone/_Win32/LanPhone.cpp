// LanPhone.cpp : Defines the entry point for the application.
//

#include <windows.h> 
#include <winsock.h>

#define IP_ADDRESS        "234.5.6.7"     
#define SOURCE_PORT       0     
#define DESTINATION_PORT  4567            

BOOL InitWS();
BOOL Send();
BOOL Receive();

int WINAPI WinMain (HINSTANCE hInstance,	// Handle to the current instance
					HINSTANCE hPrevInstance,// Handle to the previous instance
					LPTSTR lpCmdLine,		// Pointer to the command line
					int nCmdShow)			// Show window state. 
{
	if(InitWS())
		return Send();

	return FALSE;
}

BOOL InitWS()
{
	TCHAR szError[100];				// Error message string
	WSADATA WSAData;				// Contains details of the Winsock implementation

	// Initialize Winsock. 
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0) 
	{
		wsprintf (szError, TEXT("WSAStartup failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL Send()
{
	int iOptVal = 64;                   
	char szMessage[] = "Multicasting message!";	// Sent message string
	TCHAR szError[100];							// Error message string
	SOCKET Sock = INVALID_SOCKET;				// Datagram window socket
	
	SOCKADDR_IN source_sin,						// Source socket address
				dest_sin;						// Destination socket address

	// Create a datagram window socket, Sock.
	if ((Sock = socket (AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
	{
		wsprintf (szError, TEXT("Allocating socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out source socket's address information.
	source_sin.sin_family = AF_INET;
	source_sin.sin_port = htons (SOURCE_PORT);  
	source_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// Associate the source socket's address with the socket, Sock.
	if (bind (Sock, (struct sockaddr FAR *) &source_sin, sizeof (source_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Binding socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (Sock);
		return FALSE;
	}
	
	// Set the Time-to-Live of the multicast.
	if (setsockopt (Sock, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *)&iOptVal, sizeof (int)) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("setsockopt failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (Sock);
		return FALSE;
	}
	
	// Fill out the desination socket's address information.
	dest_sin.sin_family = AF_INET;
	dest_sin.sin_port = htons (DESTINATION_PORT);  
	dest_sin.sin_addr.s_addr = inet_addr (IP_ADDRESS);
	
	// Send a message to the multicasting address.
	if (sendto (Sock, szMessage, strlen (szMessage) + 1, 0, (struct sockaddr FAR *) &dest_sin, sizeof (dest_sin)) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("sendto failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (Sock);
		return FALSE;
	}
	else
		MessageBox (NULL, TEXT("Sending data succeeded!"), TEXT("Info"), MB_OK);
	
	// Disable sending on Sock before closing it.
	shutdown (Sock, 0x01);
	
	// Close Sock.
	closesocket (Sock);
	
	WSACleanup ();
	
	return TRUE;
}
					
					
					
BOOL Receive()
{
	int index = 0,					// Integer index
		iRecvLen;					// Length of recv_sin
	char szMessageA[100];			// ASCII string 
	TCHAR szMessageW[100];			// Unicode string
	TCHAR szError[100];				// Error message string
	
	SOCKET Sock = INVALID_SOCKET;	// Datagram window socket
	
	struct ip_mreq mreq;			// Used in adding or dropping 
									// multicasting addresses
	SOCKADDR_IN local_sin,			// Local socket's address
				recv_sin;			// Holds the source address upon 
									// recvfrom function returns

	// Create a datagram socket, Sock.
	if ((Sock = socket (AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
	{
		wsprintf (szError, TEXT("Allocating socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (DESTINATION_PORT);  
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// Associate the local address with Sock.
	if (bind (Sock, (struct sockaddr FAR *) &local_sin, sizeof (local_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Binding socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (Sock);
		return FALSE;
	}
	
	// Join the multicast group from which to receive datagrams.
	mreq.imr_multiaddr.s_addr = inet_addr (IP_ADDRESS);
	mreq.imr_interface.s_addr = INADDR_ANY;
	
	if (setsockopt (Sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&mreq, sizeof (mreq)) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("setsockopt failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (Sock);
		return FALSE;
	}
	
	iRecvLen = sizeof (recv_sin);
	
	// Receive data from the multicasting group server.
	if (recvfrom (Sock, szMessageA, 100, 0, (struct sockaddr FAR *) &recv_sin, &iRecvLen) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("recvfrom failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (Sock);
		return FALSE;
	}
	else
	{
		// Convert the ASCII string to a Unicode string.
		for (index = 0; index <= sizeof (szMessageA); index++)
			szMessageW[index] = szMessageA[index];
		
		MessageBox (NULL, szMessageW, TEXT("Info"), MB_OK);
	}
	
	// Disable receiving on Sock before closing it.
	shutdown (Sock, 0x00);
	
	// Close Sock.
	closesocket (Sock);
	
	WSACleanup ();
	
	return TRUE;
}
