// TcpStreamServer.h: interface for the CTcpStreamServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TcpStreamServer_H__9779FE29_CF57_4FDA_AB02_06541382276D__INCLUDED_)
#define AFX_TcpStreamServer_H__9779FE29_CF57_4FDA_AB02_06541382276D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h> 
#include <winsock.h>

#define PORTNUM               5000    // Port number  
#define MAX_PENDING_CONNECTS  4       // Maximum length of the queue 

class CTcpStreamServer  
{
public:
	CTcpStreamServer(int portnum=PORTNUM);
	virtual ~CTcpStreamServer();

	BOOL Start();
	void SetOnReceiveData(void (*func)(const char*, int));

private:
	BOOL Init();
	void Close();

private:
	int m_iPortnum;
	SOCKET m_WinSocket,  // Window socket
			m_ClientSock; // Socket for communicating 

	static DWORD WINAPI threadProc(LPVOID arg);

	void (*m_OnReceiveData)(const char *data, int size);
};

#endif // !defined(AFX_TcpStreamServer_H__9779FE29_CF57_4FDA_AB02_06541382276D__INCLUDED_)
