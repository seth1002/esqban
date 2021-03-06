// MulticastClient.h: interface for the CMulticastClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDER_H__B86EF88D_A785_474D_9BC4_93DF825869A8__INCLUDED_)
#define AFX_SENDER_H__B86EF88D_A785_474D_9BC4_93DF825869A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h> 
#include <winsock.h>

#include "..\ISocket.h"

class CMulticastClient  
{
public:
	CMulticastClient(int iSrcPort=0, int iDestPort=2000, char pszIP[]="225.6.7.8");
	virtual ~CMulticastClient();

	int Send(const char *buf, int size);

private:
	BOOL Init();
	void Close();

private:
	int m_iSourcePort;
	int m_iDestPort;
	char m_szIPAddress[15];
	SOCKET m_Sock;			// Datagram window socket
	SOCKADDR_IN source_sin,	// Source socket address
				dest_sin;	// Destination socket address
};

#endif // !defined(AFX_SENDER_H__B86EF88D_A785_474D_9BC4_93DF825869A8__INCLUDED_)
