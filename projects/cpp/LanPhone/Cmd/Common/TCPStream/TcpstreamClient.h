// TcpStreamClient.h: interface for the CTcpStreamClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSTREAMCLIENT_H__798F7DE7_84D6_4AC5_B898_3B210F866A23__INCLUDED_)
#define AFX_TCPSTREAMCLIENT_H__798F7DE7_84D6_4AC5_B898_3B210F866A23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h> 
#include <winsock.h>

#define PORTNUM         5000          // Port number
#define HOSTNAME        "localhost"   // Server name string
                                      // This should be changed
                                      // according to the server.

class CTcpStreamClient  
{
public:
	CTcpStreamClient(char *hostname=HOSTNAME, int portnum=PORTNUM);
	virtual ~CTcpStreamClient();

	int Send(const char *buf, int size);

private:
	BOOL Init();
	void Close();

private:
	char m_pszHostname[15];
	int m_iPortnum;
	SOCKET m_ServerSock; // Socket bound to the server
};

#endif // !defined(AFX_TCPSTREAMCLIENT_H__798F7DE7_84D6_4AC5_B898_3B210F866A23__INCLUDED_)
