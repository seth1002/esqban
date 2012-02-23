// SerialPort2.cpp: implementation of the CSerialPort2 class.
//
//////////////////////////////////////////////////////////////////////

#include "SerialPort.h"

#include <assert.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSerialPort::CSerialPort()
{
	m_hComm				= NULL;

	// initialize overlapped structure members to zero
	m_ov.Offset			= 0;
	m_ov.OffsetHigh		= 0;

	// create events
	m_ov.hEvent			= NULL;
	m_hWriteEvent		= NULL;
	m_hShutdownEvent	= NULL;

	m_szWriteBuffer		= NULL;

	m_bThreadAlive		= FALSE;
	m_iWriteBufferLen	= 0;
}

//
// Delete dynamic memory
//
CSerialPort::~CSerialPort()
{
	ClosePort();

	delete [] m_szWriteBuffer;
}

//
// Initialize the port. This can be port 1 to 4.
//
BOOL CSerialPort::InitPort( UINT  portnr,			// portnumber (1..4)
							UINT  baud,				// baudrate
							char  parity,			// parity 
							UINT  databits,			// databits 
							UINT  stopbits,			// stopbits 
							DWORD dwCommEvents,		// EV_RXCHAR, EV_CTS etc
							UINT  writebuffersize)	// size to the writebuffer
{
	assert(portnr > 0);// && portnr < 5);

	// if the thread is alive: Kill
	if (m_bThreadAlive) {
		do {
			SetEvent(m_hShutdownEvent);
		} while (m_bThreadAlive);
	}

	// create events
	InitEvent(&m_ov.hEvent);
	InitEvent(&m_hWriteEvent);
	InitEvent(&m_hShutdownEvent);

	// initialize the event objects
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section
	InitializeCriticalSection(&m_csCommunicationSync);
	
	// set buffersize for writing
	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portnr;

	BOOL bResult = FALSE;

	// now it critical!
	EnterCriticalSection(&m_csCommunicationSync);

	if( !OpenPortHandle(portnr) )
		return FALSE;

	// configure
	if( !SetTimeouts() ) {	
		ProcessErrorMessage("SetCommTimeouts()");
		return FALSE;
	}

	if( !SetCommMask(m_hComm, dwCommEvents) ) {
		ProcessErrorMessage("SetCommMask()");
		return FALSE;
	}

	DCB	dcb;
	if( !GetCommState(m_hComm, &dcb) ) {
		ProcessErrorMessage("GetCommState()");
		return FALSE;
	}

	dcb.fRtsControl = RTS_CONTROL_ENABLE;	// set RTS bit high!
	char szBaud[50];
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);
	if( !BuildCommDCB(szBaud, &dcb) ) {
		ProcessErrorMessage("BuildCommDCB()");
		return FALSE;
	}

	if( !SetCommState(m_hComm, &dcb) ) {
		ProcessErrorMessage("SetCommState()");
		return FALSE;
	}

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// release critical section
	LeaveCriticalSection(&m_csCommunicationSync);

	return TRUE;
}

//
//  The CommThread Function.
//
DWORD WINAPI CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of CSerialPort class
	CSerialPort *port = (CSerialPort*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	port->m_bThreadAlive = TRUE;	
		
	// Misc. variables
	DWORD BytesTransfered	= 0; 
	DWORD Event				= 0;
	DWORD CommEvent			= 0;
	DWORD dwError			= 0;
	BOOL  bResult			= TRUE;
	COMSTAT comstat;
		
	// Clear comm buffers at startup
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// begin forever loop.  This loop will run as long as the thread is alive.
	for (;;) { 

		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to 
		// be placed in a non-signeled state if there are no bytes available to be read,
		// or to a signeled state if there are bytes available.  If this event handle 
		// is set to the non-signeled state, it will be set to signeled when a 
		// character arrives at the port.

		// we do this for each port!

		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);

		if (!bResult) { 
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87:
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default:
				{
					// All other error codes indicate a serious error has
					// occured.  Process this error.
					//port->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}
		}
		else
		{
			// If WaitCommEvent() returns TRUE, check to be sure there are
			// actually bytes in the buffer to read.  
			//
			// If you are reading more than one byte at a time from the buffer 
			// (which this program does not do) you will have the situation occur 
			// where the first byte to arrive will cause the WaitForMultipleObjects() 
			// function to stop waiting.  The WaitForMultipleObjects() function 
			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
			// as it returns.  
			//
			// If in the time between the reset of this event and the call to 
			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
			// to the signeled state. When the call to ReadFile() occurs, it will 
			// read all of the bytes from the buffer, and the program will
			// loop back around to WaitCommEvent().
			// 
			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
			// but there are no bytes available to read.  If you proceed and call
			// ReadFile(), it will return immediatly due to the async port setup, but
			// GetOverlappedResults() will not return until the next character arrives.
			//
			// It is not desirable for the GetOverlappedResults() function to be in 
			// this state.  The thread shutdown event (event 0) and the WriteFile()
			// event (Event2) will not work if the thread is blocked by GetOverlappedResults().
			//
			// The solution to this is to check the buffer with a call to ClearCommError().
			// This call will reset the event handle, and if there are no bytes to read
			// we can loop back through WaitCommEvent() again, then proceed.
			// If there are really bytes to read, do nothing and proceed.
		
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}	// end if bResult

		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);

		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the higest priority and be serviced first.

			 	port->m_bThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better.
				ExitThread(0);
				break;
			}
		case 1:	// read event
			{
				GetCommMask(port->m_hComm, &CommEvent);
				if (CommEvent & EV_CTS)
					port->m_OnReceiveStatus(EV_CTS);
				if (CommEvent & EV_RXFLAG)
					port->m_OnReceiveStatus(EV_RXFLAG);
				if (CommEvent & EV_BREAK)
					port->m_OnReceiveStatus(EV_BREAK);
				if (CommEvent & EV_ERR)
					port->m_OnReceiveStatus(EV_ERR);
				if (CommEvent & EV_RING)
					port->m_OnReceiveStatus(EV_RING);
				
				if (CommEvent & EV_RXCHAR)
					// Receive character event from port.
					ReceiveChar(port, comstat);
					
				break;
			}  
		case 2: // write event
			{
				// Write character event from port
				WriteChar(port);
				break;
			}

		} // end switch

	} // close forever loop

	return 0;
}

//
// start comm watching
//
BOOL CSerialPort::StartMonitoring()
{
    m_hThread = CreateThread( 
					NULL,				// no security attributes 
					0,					// use default stack size  
					CommThread,			// thread function 
					this,				// argument to thread function 
					0,					// use default creation flags 
					&m_dwThreadId);		// returns the thread identifier 

	return (m_hThread != NULL);
}

//
// Restart the comm thread
//
BOOL CSerialPort::RestartMonitoring()
{
	return( ResumeThread(m_hThread) != -1 );
}


//
// Suspend the comm thread
//
BOOL CSerialPort::StopMonitoring()
{
	return( SuspendThread(m_hThread) != -1 );
}


//
// If there is a error, give the right message
//
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

//
// Write a character.
//
void CSerialPort::WriteChar(CSerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	ResetEvent(port->m_hWriteEvent);

	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);

	if (bWrite) {
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(port->m_hComm,				// Handle to COMM Port
							port->m_szWriteBuffer,		// Pointer to message buffer in calling finction
							port->m_iWriteBufferLen,	// Length of message to send
							&BytesSent,					// Where to store the number of bytes sent
							&port->m_ov);				// Overlapped structure

		// deal with any error codes
		if (!bResult) {
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage("WriteFile()");
					}
			}
		} 
		else {
			LeaveCriticalSection(&port->m_csCommunicationSync);
		}
	} // end if(bWrite)

	if (!bWrite) {
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
									  &port->m_ov,		// Overlapped structure
									  &BytesSent,		// Stores number of bytes sent
									  TRUE); 			// Wait flag

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
		if (!bResult) {
			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	} // end if (!bWrite)

	// Verify that the data size send equals what we tried to send
//	if (BytesSent != port->m_iWriteBufferLen)
//	{
//		printf("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, port->m_iWriteBufferLen);
//	}
}

//
// Character received. Inform the owner
//
void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff;

	for (;;) { 
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solutiion to be the most efficient way to do this.
		
		if (comstat.cbInQue == 0) {
			// break out when all bytes have been read
			break;
		}
						
		EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead) {
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
							   &RXBuff,				// RX Buffer Pointer
							   1,					// Read one byte
							   &BytesRead,			// Stores number of bytes read
							   &port->m_ov);		// pointer to the m_ov structure
			// deal with the error code 
			if (!bResult) { 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress 
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{
							// Another error has occured.  Process this error.
							port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else {
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)

		if (!bRead) {
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
										  &port->m_ov,		// Overlapped structure
										  &BytesRead,		// Stores number of bytes read
										  TRUE); 			// Wait flag

			// deal with the error code 
			if (!bResult) {
				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}  // close if (!bRead)
				
		LeaveCriticalSection(&port->m_csCommunicationSync);

		// notify parent that a byte was received
//		::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
		//port->OnReceiveChar(RXBuff);
		if(port->m_OnReceiveChar)
			port->m_OnReceiveChar(RXBuff);
	} // end forever loop

}

//
// Write a string to the port
//
void CSerialPort::WriteToPort(char* string, int len)
{		
//	assert(m_hComm != 0);
	if(m_hComm == NULL)
		return;

	m_iWriteBufferLen = len;
	strnset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strncpy(m_szWriteBuffer, string, len);

	// set event for write
	SetEvent(m_hWriteEvent);
}

BOOL CSerialPort::OpenPortHandle(int port)
{
	char szPort[5] = {0};
	sprintf(szPort, "COM%d", port);

	// if the port is already opened: close it
	if (m_hComm != NULL) {
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// get a handle to the port
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	return( m_hComm != INVALID_HANDLE_VALUE );
}

BOOL CSerialPort::SetTimeouts()
{
	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout			= 0xFFFFFFFF;
	cto.ReadTotalTimeoutMultiplier	= 0;
	cto.ReadTotalTimeoutConstant	= 0;
	cto.WriteTotalTimeoutMultiplier	= 0;
	cto.WriteTotalTimeoutConstant	= 5000;
	return SetCommTimeouts(m_hComm, &cto);
}

inline void CSerialPort::InitEvent(HANDLE *hEvent)
{
	if( *hEvent != NULL )
		ResetEvent( *hEvent );
	*hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CSerialPort::ClosePort()
{
	do {
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);
}

void CSerialPort::SendData( const char *data, int len)
{
	WriteToPort((char*)data, len);
}

void CSerialPort::SendChar(char c)
{
	if(m_hComm)
		BOOL bResult = TransmitCommChar(m_hComm, c);
}

void CSerialPort::Purge()
{
	// Clear comm buffers
	if (m_hComm)		// check if the port is opened
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

void CSerialPort::Reset()
{
	if(!m_hComm)
		return;
	BOOL bResult;
//	bResult = EscapeCommFunction(m_hComm, CLRDTR);	// Clears the DTR (data-terminal-ready) signal.
//	bResult = EscapeCommFunction(m_hComm, CLRRTS);	// Clears the RTS (request-to-send) signal.
	bResult = EscapeCommFunction(m_hComm, SETDTR);	// Sends the DTR (data-terminal-ready) signal.
	bResult = EscapeCommFunction(m_hComm, SETRTS);	// Sends the RTS (request-to-send) signal.
//	bResult = EscapeCommFunction(m_hComm, SETXOFF);	// Causes transmission to act as if an XOFF character has been received.
//	bResult = EscapeCommFunction(m_hComm, SETXON);	// Causes transmission to act as if an XON character has been received.
//	bResult = EscapeCommFunction(m_hComm, SETBREAK);// Suspends character transmission and places the transmission line in a break state until the ClearCommBreak function is called (or EscapeCommFunction is called with the CLRBREAK extended function code). The SETBREAK extended function code is identical to the SetCommBreak function. Note that this extended function does not flush data that has not been transmitted.
//	bResult = EscapeCommFunction(m_hComm, CLRBREAK);// Restores character transmission and places the transmission line in a nonbreak state. The CLRBREAK extended function code is identical to the ClearCommBreak function.
}

void CSerialPort::SetOnReceiveChar(void (*func)(const char ch))
{
	m_OnReceiveChar = func;
}

void CSerialPort::SetOnReceiveStatus(void (*func)(int status))
{
	m_OnReceiveStatus = func;
}
