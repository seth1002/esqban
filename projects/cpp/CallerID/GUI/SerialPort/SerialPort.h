// SerialPort2.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT2_H__50480AF5_7640_4D7C_A305_AC5F486F2FE9__INCLUDED_)
#define AFX_SERIALPORT2_H__50480AF5_7640_4D7C_A305_AC5F486F2FE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CSerialPort
{
public:
	CSerialPort();
	virtual ~CSerialPort();

	BOOL InitPort(	UINT port=1, UINT baud=9600, char parity='N', UINT data=8, 
					UINT stop=1, DWORD events=EV_RXCHAR|EV_CTS, UINT buffsize=512 );
	void ClosePort();

	BOOL StartMonitoring();
	BOOL RestartMonitoring();
	BOOL StopMonitoring();

	void SendData( const char *data, int length );
	void SendChar(char c);

	void SetOnReceiveChar(void (*func)(const char ch));
	void (*m_OnReceiveChar)(const char);

	void SetOnReceiveStatus(void (*func)(int status));
	void (*m_OnReceiveStatus)(int);

protected:
	void Purge();
	void Reset();

private:
	void WriteToPort(char* string, int len);

	BOOL SetTimeouts();
	BOOL OpenPortHandle(int port);
	void ProcessErrorMessage(char* ErrorText);
	static DWORD WINAPI	CommThread(LPVOID pParam);
	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);
	static void	WriteChar(CSerialPort* port);
	inline void InitEvent(HANDLE *hEvent);

private:
	// thread
	DWORD	m_dwThreadId;
	HANDLE	m_hThread;

	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;
	BOOL				m_bThreadAlive;

	// handles
	HANDLE	m_hShutdownEvent;
	HANDLE	m_hComm;
	HANDLE	m_hWriteEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the 
	// overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	HANDLE	m_hEventArray[3];

	// structures
	OVERLAPPED	m_ov;

	// misc
	UINT	m_nPortNr;
	char*	m_szWriteBuffer;
	DWORD	m_dwCommEvents;
	int		m_iWriteBufferLen;
};

#endif // !defined(AFX_SERIALPORT2_H__50480AF5_7640_4D7C_A305_AC5F486F2FE9__INCLUDED_)
