// Receiver.h: interface for the CReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECEIVER_H__C8E43392_DD61_4018_8962_E754030002C9__INCLUDED_)
#define AFX_RECEIVER_H__C8E43392_DD61_4018_8962_E754030002C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h> 
#include <winsock.h>

class CReceiver  
{
public:
	CReceiver(int iSrcPort=0, int iDestPort=2000, char *pszIP=NULL);
	virtual ~CReceiver();

	BOOL Start();
	void SetOnReceiveData(void (*func)(const char*, int));

private:
	BOOL Init(int iSrcPort, int iDestPort, char *pszIP);
	void Close();
	static void ErrorHandlerEx( WORD wLine, LPSTR lpszFile );

private:
	//int m_iDestPort;
	//char m_szIPAddress[15];
	SOCKET m_Sock;			// Datagram window socket
	SOCKADDR_IN local_sin,	// Local socket's address
				recv_sin;	// Holds the source address upon 
							// recvfrom function returns

	static DWORD WINAPI threadProc(LPVOID arg);

	void (*m_OnReceiveData)(const char *data, int size);
};

#endif // !defined(AFX_RECEIVER_H__C8E43392_DD61_4018_8962_E754030002C9__INCLUDED_)
