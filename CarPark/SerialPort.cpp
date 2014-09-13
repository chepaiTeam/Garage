#include "StdAfx.h"

//volatile BOOL m_bRun;
//CSerialPort::CSerialPort(void)
//{
//	m_hComm = NULL;
//	// initialize overlapped structure members to zero
//	m_ov.Offset = 0;
//	m_ov.OffsetHigh = 0;
//
//	// create events
//	m_ov.hEvent = NULL;
//	m_hWriteEvent = NULL;
//	m_hShutdownEvent = NULL;
//
//
//	m_szReadBuffer = NULL;
//	m_nReadPosition = 0;
//
//	m_szWriteBuffer = NULL;
//	m_nWriteSize = 0;
//
//	m_bIsRead = FALSE;
//	m_bIsWrite = FALSE;
//
//	m_bThreadAlive = FALSE;
//
//	//m_nReadBufferLen = 0;
//}
//
//
//CSerialPort::~CSerialPort(void)
//{
//	do
//	{
//		SetEvent(m_hShutdownEvent);
//	} while (m_bThreadAlive);
//
//
//	// if the port is still opened: close it 
//	if (m_hComm != NULL)
//	{
//		CloseHandle(m_hComm);
//		m_hComm = NULL;
//	}
//	// Close Handles  
//	if(m_hShutdownEvent!=NULL)
//		CloseHandle( m_hShutdownEvent); 
//	if(m_ov.hEvent!=NULL)
//		CloseHandle( m_ov.hEvent ); 
//	if(m_hWriteEvent!=NULL)
//		CloseHandle( m_hWriteEvent ); 
//
//	//TRACE("Thread ended\n");
//
//	delete [] m_szReadBuffer;
//	delete [] m_szWriteBuffer;
//
//}
//
//
//BOOL CSerialPort::InitSerialPort(CString PortName, UINT BaudRate, char Parity, UINT Databits, UINT Stopbits, DWORD dwCommEvents, UINT ReadWriteBufferSize)
//{
//	CString strBaud;
//	m_nPortName = PortName.GetBuffer();
//
//	// create events
//	if (m_ov.hEvent != NULL)
//		ResetEvent(m_ov.hEvent);
//	else
//		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//	if (m_hWriteEvent != NULL)
//		ResetEvent(m_hWriteEvent);
//	else
//		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//	if (m_hShutdownEvent != NULL)
//		ResetEvent(m_hShutdownEvent);
//	else
//		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//	// initialize the event objects
//	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
//	m_hEventArray[1] = m_ov.hEvent;
//	m_hEventArray[2] = m_hWriteEvent;
//
//	// initialize critical section
//	InitializeCriticalSection(&m_csCommunicationSync);
//
//	// set buffersize for writing and save the owner
//	//m_pOwner = pPortOwner;
//
//
//	m_nReadBufferSize = m_nWriteBufferSize = ReadWriteBufferSize;
//
//	if (m_szReadBuffer != NULL)
//		delete [] m_szReadBuffer;
//	m_szReadBuffer = new char[m_nReadBufferSize];
//
//	if (m_szWriteBuffer != NULL)
//		delete [] m_szWriteBuffer;
//	m_szWriteBuffer = new char[m_nWriteBufferSize];
//
//	m_dwCommEvents = dwCommEvents;
//
//	// now it critical!
//	EnterCriticalSection(&m_csCommunicationSync);
//
//	// if the port is already opened: close it
//	if (m_hComm != NULL)
//	{
//		CloseHandle(m_hComm);
//		m_hComm = NULL;
//	}
//
//	strBaud.Format(_T("baud=%d parity=%c data=%d stop=%d"), BaudRate, Parity, Databits, Stopbits);
//
//	// get a handle to the port
//	m_hComm = CreateFile(PortName,						// communication port string (COMX)
//		GENERIC_READ | GENERIC_WRITE,					// read/write types
//		NULL,											// comm devices must be opened with exclusive access
//		NULL,											// no security attributes
//		OPEN_EXISTING,									// comm devices must use OPEN_EXISTING
//		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,		// Async I/O
//		NULL);											// template must be 0 for comm devices
//
//	if (m_hComm == INVALID_HANDLE_VALUE)
//	{
//		ProcessErrorMessage("打开串口失败！");
//		return FALSE;
//	}
//
//	// set the timeout values
//	m_CommTimeouts.ReadIntervalTimeout = -1;
//	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
//	m_CommTimeouts.ReadTotalTimeoutConstant = 0;
//	m_CommTimeouts.WriteTotalTimeoutMultiplier = 100;
//	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;
//
//	//SYSTEMTIME m_sysTime;
//	//::GetLocalTime(&m_sysTime);
//	//if (m_sysTime.wYear>=2013 && m_sysTime.wMonth>= 5 && m_sysTime.wDay >= 1)
//	//{
//	//	return FALSE;
//	//}
//
//	// configure
//	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
//	{						   
//		if (SetCommMask(m_hComm, dwCommEvents))
//		{
//			//if (GetCommState(m_hComm, &m_dcb))
//			//{
//			//	m_dcb.EvtChar = 'q';
//			//	m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
//			//	if (BuildCommDCB(strBaud, &m_dcb))
//			//	{
//			//		if (SetCommState(m_hComm, &m_dcb))
//			//			;//ProcessErrorMessage("SetCommState() OK!"); // normal operation... continue
//			//		else
//			//			ProcessErrorMessage("SetCommState()");
//			//	}
//			//	else
//			//		ProcessErrorMessage("BuildCommDCB()");
//			//}
//			//else
//			//	ProcessErrorMessage("GetCommState()");
//		}
//		else
//			ProcessErrorMessage("SetCommMask()");
//	}
//	else
//		ProcessErrorMessage("SetCommTimeouts()");
//
//	SetupComm(m_hComm, m_nReadBufferSize, m_nWriteBufferSize); 
//
//	// flush the port
//	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
//	//PurgeComm(m_hComm, PURGE_RXCLEAR);
//
//	// release critical section
//	LeaveCriticalSection(&m_csCommunicationSync);
//	return TRUE;
//}
//
////
////  The CommThread Function.
////
//UINT CSerialPort::CommThread(LPVOID pParam)
//{
//	// Cast the void pointer passed to the thread back to a pointer of CSerialPort class
//	CSerialPort *port = (CSerialPort*)pParam;
//
//	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
//	port->m_bThreadAlive = TRUE;	
//	// Misc. variables
//
//	DWORD BytesTransfered = 0; 
//	DWORD Event = 0;
//	DWORD CommEvent = 0;
//	DWORD dwError = 0;
//	COMSTAT comstat;
//	memset(&comstat, 0, sizeof(COMSTAT));
//	BOOL  bResult = TRUE;
//
//	// Clear comm buffers at startup
//	//if (port->m_hComm)		// check if the port is opened 检查端口是否打开
//	//	PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
//	PurgeComm(port->m_hComm, PURGE_RXCLEAR);
//
//	// begin forever loop.  This loop will run as long as the thread is alive.
//	while(port->m_bThreadAlive)
//	{
//		// Make a call to WaitCommEvent().  This call will return immediatly
//		// because our port was created as an async port (FILE_FLAG_OVERLAPPED
//		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
//		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to 
//		// be placed in a non-signeled state if there are no bytes available to be read,
//		// or to a signeled state if there are bytes available.  If this event handle 
//		// is set to the non-signeled state, it will be set to signeled when a 
//		// character arrives at the port.
//
//		// we do this for each port!
//
//
//		//if (port->m_ov.hEvent != NULL)
//		//	ResetEvent(port->m_ov.hEvent);
//		//else
//		//	port->m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//		////if (port->m_hWriteEvent != NULL)
//		////	ResetEvent(port->m_hWriteEvent);
//		////else
//		////	port->m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//		//if (port->m_hShutdownEvent != NULL)
//		//	ResetEvent(port->m_hShutdownEvent);
//		//else
//		//	port->m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//		//// initialize the event objects
//		//port->m_hEventArray[0] = port->m_hShutdownEvent;	// highest priority
//		//port->m_hEventArray[1] = port->m_ov.hEvent;
//		////port->m_hEventArray[2] = port->m_hWriteEvent;
//
//		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);
//
//		if (!bResult)  
//		{ 
//			// If WaitCommEvent() returns FALSE, process the last error to determin
//			// the reason..
//			switch (dwError = GetLastError()) 
//			{ 
//			case ERROR_IO_PENDING: 	
//				{ 
//					// This is a normal return value if there are no bytes
//					// to read at the port.
//					// Do nothing and continue
//					break;
//				}
//			case 87:
//				{
//					// Under Windows NT, this value is returned for some reason.
//					// I have not investigated why, but it is also a valid reply
//					// Also do nothing and continue.
//					break;
//				}
//			default:
//				{
//					// All other error codes indicate a serious error has
//					// occured.  Process this error.
//					port->ProcessErrorMessage("WaitCommEvent()");
//					break;
//				}
//			}
//		}
//		else
//		{
//			// If WaitCommEvent() returns TRUE, check to be sure there are actually bytes in the buffer to read.  
//			//
//			// If you are reading more than one byte at a time from the buffer 
//			// (which this program does not do) you will have the situation occur 
//			// where the first byte to arrive will cause the WaitForMultipleObjects() 
//			// function to stop waiting.  The WaitForMultipleObjects() function 
//			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
//			// as it returns.  
//			//
//			// If in the time between the reset of this event and the call to 
//			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
//			// to the signeled state. When the call to ReadFile() occurs, it will 
//			// read all of the bytes from the buffer, and the program will
//			// loop back around to WaitCommEvent().
//			// 
//			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
//			// but there are no bytes available to read.  If you proceed and call
//			// ReadFile(), it will return immediatly due to the async port setup, but
//			// GetOverlappedResults() will not return until the next character arrives.
//			//
//			// It is not desirable for the GetOverlappedResults() function to be in 
//			// this state.  The thread shutdown event (event 0) and the WriteFile()
//			// event (Event2) will not work if the thread is blocked by GetOverlappedResults().
//			//
//			// The solution to this is to check the buffer with a call to ClearCommError().
//			// This call will reset the event handle, and if there are no bytes to read
//			// we can loop back through WaitCommEvent() again, then proceed.
//			// If there are really bytes to read, do nothing and proceed.
//
//			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
//
//			if (comstat.cbInQue == 0){
//				continue;
//			}
//		}	// end if bResult
//
//		// Main wait function.  This function will normally block the thread until one of nine events occur that require action.
//		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);
//
//		switch (Event)
//		{
//		case 0:
//			{
//				// Shutdown event.  This is event zero so it will be the higest priority and be serviced first.
//				CloseHandle(port->m_hComm);
//				port->m_hComm=NULL;
//				port->m_bThreadAlive = FALSE;
//
//				// Kill this thread.  break is not needed, but makes me feel better.
//				ExitThread(0);
//
//				break;
//			}
//		case 1:	// read event
//			{
//				GetCommMask(port->m_hComm, &CommEvent);
//				//if (CommEvent & EV_CTS)
//				//	LOG4CPLUS_WARN(root, _T("WM_COMM_CTS_DETECTED"));
//				////	::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
//				//if (CommEvent & EV_BREAK)
//				//	LOG4CPLUS_WARN(root, _T("WM_COMM_BREAK_DETECTED"));
//				////	::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
//				//if (CommEvent & EV_ERR)
//				//	LOG4CPLUS_WARN(root, _T("WM_COMM_ERR_DETECTED"));
//				////	::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
//				//if (CommEvent & EV_RING)
//				//	LOG4CPLUS_WARN(root, _T("WM_COMM_RING_DETECTED"));
//				////	::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
//
//				//if (CommEvent & EV_RXFLAG)
//				//	//LOG4CPLUS_WARN(root, _T("WM_COMM_RXFLAG_DETECTED"));
//				//	ReceiveChar(port, comstat);
//				////	::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
//
//				if (CommEvent & EV_RXCHAR)
//					// Receive character event from port.
//					ReceiveChar(port, comstat);
//
//				break;
//			}  
//		case 2: // write event
//			{
//				// Write character event from port
//				WriteChar(port);
//				break;
//			}
//
//		} // end switch
//
//	} // close forever loop
//
//	return 0;
//}
//
////
//// start comm watching
////
//BOOL CSerialPort::StartMonitoring()
//{
//	LOG4CPLUS_INFO(root, _T("Thread started"));
//
//	// TODO: Add your control notification handler code here
//	hThread = CreateThread(NULL,
//		0,
//		(LPTHREAD_START_ROUTINE)CommThread,
//		this/*NULL*/,
//		0,
//		&ThreadID);
//
//	return TRUE;	
//}
//
//
////
//// Suspend the comm thread
////
//BOOL CSerialPort::StopMonitoring()
//{
//	LOG4CPLUS_INFO(root, _T("Thread suspended"));
//
//	m_bThreadAlive=FALSE;
//
//	return TRUE;	
//}
//
//
////
//// If there is a error, give the right message
////
//void CSerialPort::ProcessErrorMessage(CString ErrorText)
//{
//	CString strMsg;
//	LPSTR lpMsgBuf=NULL;
//	FormatMessage( 
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL,
//		GetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
//		/*(LPTSTR) &*/lpMsgBuf,
//		0,
//		NULL 
//		);
//
//	strMsg.Format(_T("WARNING:  %s \nFailed with the following error: \n%s\nPort: %s\n"), ErrorText, lpMsgBuf, m_nPortName);
//	
//	//LOG4CPLUS_ERROR(root, strMsg);
//	//MessageBox(m_hWnd, strMsg, L"Application Error", MB_ICONSTOP);
//}
//
////
//// Write a character.
////
//void CSerialPort::WriteChar(CSerialPort* port)
//{
//	BOOL bWrite = TRUE;
//	BOOL bResult = TRUE;
//
//	DWORD BytesSent = 0;
//
//	port->m_bIsWrite = TRUE;
//
//	ResetEvent(port->m_hWriteEvent);
//
//	// Gain ownership of the critical section
//	EnterCriticalSection(&port->m_csCommunicationSync);
//
//	if (bWrite)
//	{
//		// Initailize variables
//		port->m_ov.Offset = 0;
//		port->m_ov.OffsetHigh = 0;
//
//		// Clear buffer
//		//PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
//		//PurgeComm(port->m_hComm, PURGE_RXCLEAR);
//
//		bResult = WriteFile(port->m_hComm,			// Handle to COMM Port
//			port->m_szWriteBuffer,					// Pointer to message buffer in calling finction
//			port->m_nWriteSize,						// Length of message to send
//			&BytesSent,								// Where to store the number of bytes sent
//			&port->m_ov);							// Overlapped structure
//
//		// deal with any error codes
//		if (!bResult)  
//		{
//			DWORD dwError = GetLastError();
//			switch (dwError)
//			{
//			case ERROR_IO_PENDING:
//				{
//					// continue to GetOverlappedResults()
//					BytesSent = 0;
//					bWrite = FALSE;
//					break;
//				}
//			default:
//				{
//					// all other error codes
//					port->ProcessErrorMessage("WriteFile()");
//				}
//			}
//		} 
//		else
//		{
//			LeaveCriticalSection(&port->m_csCommunicationSync);
//		}
//	} // end if(bWrite)
//
//	if (!bWrite)
//	{
//		bWrite = TRUE;
//
//		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
//			&port->m_ov,								// Overlapped structure
//			&BytesSent,									// Stores number of bytes sent
//			TRUE); 										// Wait flag
//
//		LeaveCriticalSection(&port->m_csCommunicationSync);
//
//		// deal with the error code 
//		if (!bResult)  
//		{
//			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
//		}
//	} // end if (!bWrite)
//
//	//Verify that the data size send equals what we tried to send
//	if (BytesSent != port->m_nWriteSize)	// Length of message to send)
//	{
//		//TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
//	}
//
//
//	port->m_bIsWrite = FALSE;
//	//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
//	//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED,0,(LPARAM) port->m_nPortNr);
//}
//
////
//// Character received. Inform the owner
////
//void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
//{
//	BOOL  bRead = TRUE; 
//	BOOL  bResult = TRUE;
//	DWORD dwError = 0;
//	DWORD BytesRead = 0;
//	//char readBuffer;
//
//	DWORD cbInQueOld=0;
//
//	port->m_bIsRead = TRUE;
//
//	//LOG4CPLUS_INFO(root, _T("GO"));
//
//	while(true) 
//	{ 
//		//add by liquanhai  防止死锁   2011-11-06
//		//if(WaitForSingleObject(port->m_hShutdownEvent,0) == WAIT_OBJECT_0)
//		//	return;
//
//		Sleep(200);
//		//ResetEvent(port->m_ov.hEvent);
//
//		// Gain ownership of the comm port critical section.
//		// This process guarantees no other part of this program is using the port object. 
//
//		EnterCriticalSection(&port->m_csCommunicationSync);
//
//		// ClearCommError() will update the COMSTAT structure and clear any other errors.
//
//		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
//		//cbInQueOld = comstat.cbInQue;
//		//Sleep(5);
//		//bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
//
//		LeaveCriticalSection(&port->m_csCommunicationSync);
//
//		//if (cbInQueOld < comstat.cbInQue)
//		//{
//		//	continue;
//		//}
//
//		// start forever loop.  I use this type of loop because I do not know at runtime how many loops this will have to run. 
//		// My solution is to start a forever loop and to break out of it when I have processed all of the data available. 
//		// Be careful with this approach and be sure your loop will exit.
//		// My reasons for this are not as clear in this sample as it is in my production code, but I have found this solutiion to be the most efficient way to do this.
//
//		if (comstat.cbInQue == 0)// || number == 10
//		{
//			// break out when all bytes have been read
//			ResetEvent(port->m_ov.hEvent);
//			ResetEvent(port->m_hEventArray[1]);
//			break;
//		}
//		EnterCriticalSection(&port->m_csCommunicationSync);
//
//		if (bRead)
//		{
//			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
//				port->m_szReadBuffer,						// RX Buffer Pointer
//				comstat.cbInQue,									// Read one byte
//				&BytesRead,							// Stores number of bytes read
//				&port->m_ov);						// pointer to the m_ov structure
//			// deal with the error code 
//			if (!bResult)  
//			{ 
//				switch (dwError = GetLastError()) 
//				{ 
//				case ERROR_IO_PENDING: 	
//					{ 
//						// asynchronous i/o is still in progress 
//						// Proceed on to GetOverlappedResults();
//						bRead = FALSE;
//						break;
//					}
//				default:
//					{
//						// Another error has occured.  Process this error.
//						port->ProcessErrorMessage("ReadFile()");
//						break;
//					} 
//				}
//			}
//			else
//			{
//				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
//
//				bRead = TRUE;
//			}
//		}  // close if (bRead)
//
//		if (!bRead)
//		{
//			bRead = TRUE;
//			bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
//				&port->m_ov,								// Overlapped structure
//				&BytesRead,									// Stores number of bytes read
//				TRUE); 										// Wait flag
//
//			// deal with the error code 
//			if (!bResult)  
//			{
//				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
//			}	
//		}  // close if (!bRead)
//
//		LeaveCriticalSection(&port->m_csCommunicationSync);
//
//
//		//EnterCriticalSection(&port->m_csCommunicationSync);
//
//		//if (bRead)
//		//{
//		//	bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
//		//		&readBuffer,						// RX Buffer Pointer
//		//		1,									// Read one byte
//		//		&BytesRead,							// Stores number of bytes read
//		//		&port->m_ov);						// pointer to the m_ov structure
//		//	// deal with the error code 
//		//	if (!bResult)  
//		//	{ 
//		//		switch (dwError = GetLastError()) 
//		//		{ 
//		//		case ERROR_IO_PENDING: 	
//		//			{ 
//		//				// asynchronous i/o is still in progress 
//		//				// Proceed on to GetOverlappedResults();
//		//				bRead = FALSE;
//		//				break;
//		//			}
//		//		default:
//		//			{
//		//				// Another error has occured.  Process this error.
//		//				port->ProcessErrorMessage("ReadFile()");
//		//				break;
//		//			} 
//		//		}
//		//	}
//		//	else
//		//	{
//		//		// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
//
//		//		LOG4CPLUS_INFO(root, _T("OK"));
//		//		bRead = TRUE;
//		//	}
//		//}  // close if (bRead)
//
//		//if (!bRead)
//		//{
//		//	bRead = TRUE;
//		//	bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
//		//		&port->m_ov,								// Overlapped structure
//		//		&BytesRead,									// Stores number of bytes read
//		//		TRUE); 										// Wait flag
//
//		//	// deal with the error code 
//		//	if (!bResult)  
//		//	{
//		//		port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
//		//	}	
//		//}  // close if (!bRead)
//
//		//LeaveCriticalSection(&port->m_csCommunicationSync);
//
//
//		//port->m_szReadBuffer[port->m_nReadPosition] = readBuffer;
//		//if (port->m_nReadPosition < port->m_nReadBufferSize - 1)
//		//{
//		//	port->m_nReadPosition ++;
//		//} 
//		//else
//		//{
//		//	memset(port->m_szReadBuffer, 0, port->m_nReadBufferSize);
//		//	port->m_nReadPosition = 0;
//		//}
//
//		// notify parent that a byte was received
//		//::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
//	} // end forever loop
//
//	port->m_bIsRead = FALSE;
//}
//
//////
////// Write a string to the port
//////
////void CSerialPort::WriteToPort(char* string)
////{		
////	//assert(m_hComm != 0);
////	//m_bIsWrite = TRUE;
////
////	//memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
////	//strcpy(m_szWriteBuffer, string);
////	//m_nWriteSize=strlen(string);
////
////	//// set event for write
////	//SetEvent(m_hWriteEvent);
////}
////
////
////void CSerialPort::WriteToPort(char* string,int nLen)
////{		
////	assert(m_hComm != 0);
////	m_bIsWrite = TRUE;
////
////	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
////	//	memset(m_szWriteBuffer, 0, nLen);
////	//	strncpy(m_szWriteBuffer, string, nLen);
////	memcpy(m_szWriteBuffer, string, nLen);
////	m_nWriteSize=nLen;
////
////	// set event for write
////	SetEvent(m_hWriteEvent);
////}
////
////void CSerialPort::WriteToPort(CString string)
////{		
////	assert(m_hComm != 0);
////	m_bIsWrite = TRUE;
////
////	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
////	//strcpy(m_szWriteBuffer, string);
////	//memcpy(m_szWriteBuffer, string.GetBuffer(string.GetLength()), string.GetLength());
////	for(int i=0;i<string.GetLength();i++)
////	{
////		m_szWriteBuffer[i]=(char)string.GetAt(i);
////	}
////	m_nWriteSize=string.GetLength();
////
////	// set event for write
////	SetEvent(m_hWriteEvent);
////}
////
////void CSerialPort::WriteToPort(LPCTSTR string,int n)
////{		
////	assert(m_hComm != 0);
////	m_bIsWrite = TRUE;
////
////	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
////	//	strncpy(m_szWriteBuffer, string, n);
////	memcpy(m_szWriteBuffer, string, n);
////	m_nWriteSize=n;
////	// set event for write
////	SetEvent(m_hWriteEvent);
////}
//
//
//
//void CSerialPort::WriteToPort(BYTE* nByte, int nLen)
//{
//	assert(m_hComm != 0);
//	do{
//		;//Sleep(10);
//	}while (m_bIsWrite);//m_bIsRead || 
//	m_bIsWrite = TRUE;
//
//	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
//
//	memcpy(m_szWriteBuffer, nByte, nLen);
//	m_nWriteSize = nLen;
//
//	// set event for write
//	SetEvent(m_hWriteEvent);
//}
//
//
//DWORD CSerialPort::ReadFormPort(BYTE* nByte, int nLen)
//{
//	assert(m_hComm != 0);
//	DWORD Len = 0;
//
//	memset(nByte, 0, nLen);
//	//if (m_nReadBufferLen>=nLen)
//	{
//		//Len = m_nReadBufferLen+1;
//		memcpy(nByte, m_szReadBuffer, nLen);
//	} 
//	//m_nReadBufferLen = 0;					//移动数据指针到首地址
//
//	return Len;
//}
//
////
//// Return the device control block
////
//DCB CSerialPort::GetDCB()
//{
//	return m_dcb;
//}
//
////
//// Return the communication event masks
////
//DWORD CSerialPort::GetCommEvents()
//{
//	return m_dwCommEvents;
//}
//
////
//// Return the output buffer size
////
//DWORD CSerialPort::GetWriteBufferSize()
//{
//	return m_nWriteBufferSize;
//}
//
//
//void CSerialPort::ClosePort(void)
//{
//	SetEvent(m_hShutdownEvent);
//}


CSerialPort::CSerialPort()
{
	m_hFile = NULL;
	m_bThreadAlive = FALSE;
	m_hEventCloseThread = NULL;

	m_szReadBuffer = NULL;
	m_nReadPosition = 0;
}

CSerialPort::~CSerialPort()
{
	Close();

	if (m_szReadBuffer != NULL)
		delete [] m_szReadBuffer;
	m_szReadBuffer = NULL;
}

BOOL CSerialPort::Open(CString PortName, UINT BaudRate, char Parity, UINT Databits, UINT Stopbits, DWORD dwCommEvents, UINT ReadWriteBufferSize)
{
	//_ASSERTE(uiPortNo>=0 && uiPortNo<MAX_SCC_NUM);
	//_ASSERTE(uiPortNo>=0 && uiPortNo<255);
	//_ASSERTE(pWndReceMsg!=NULL);

	DCB dcb;
	COMMTIMEOUTS cto;
	CString strBaud;

	m_nReadBufferSize = ReadWriteBufferSize;
	if (m_szReadBuffer != NULL)
		delete [] m_szReadBuffer;
	m_szReadBuffer = new char[m_nReadBufferSize];

	if(m_bThreadAlive||m_hFile)			return FALSE;
	
	m_hFile = ::CreateFile(PortName.GetBuffer(),
		GENERIC_READ|GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		0);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		switch (::GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			//strcat(str," 为无效端口！");
			//AfxMessageBox(str);
			break;
		case ERROR_ACCESS_DENIED:
			//strcat(str," 已被其他程序打开！");
			//AfxMessageBox(str);
			break;
		default:
			//strcat(str," 打开失败！未知错误！");
			//AfxMessageBox(str);
			break;
		}
		m_hFile = NULL;

		return FALSE;
	}

	if (m_hEventCloseThread != NULL)	ResetEvent(m_hEventCloseThread);
	m_hEventCloseThread = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hEventCloseThread == 0)	
	{
		::CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}
	//====

	//Config
	strBaud.Format(_T("baud=%d parity=%c data=%d stop=%d"), BaudRate, Parity, Databits, Stopbits);

	dcb.DCBlength = sizeof(DCB);
	if (!::GetCommState(m_hFile,&dcb))	return FALSE;
	if (!::BuildCommDCB(strBaud, &dcb))	return FALSE;
	if (!::SetCommState(m_hFile, &dcb))	return FALSE;

	if (!::SetupComm(m_hFile, ReadWriteBufferSize, ReadWriteBufferSize))	return FALSE;
	if (!::SetCommMask(m_hFile, dwCommEvents))	return FALSE;
	if (!::GetCommTimeouts(m_hFile, &cto))	return FALSE;
	cto.ReadIntervalTimeout = -1;			// 读间隔超时。 接收时，两字符间最大的时延。
	cto.ReadTotalTimeoutConstant = 20;		// 读时间系数。 读取每字节的超时。
	cto.ReadTotalTimeoutMultiplier = 500;		// 读时间常量。 读串口数据的固定超时。
											// 总超时 = ReadTotalTimeoutMultiplier * 字节数 + ReadTotalTimeoutConstant  
	cto.WriteTotalTimeoutConstant =100;		// 写时间系数。 写每字节的超时。
	cto.WriteTotalTimeoutMultiplier =100;	// 写时间常量。 写串口数据的固定超时。
											// 总超时 = WriteTotalTimeoutMultiplier * 字节数 + WriteTotalTimeoutConstant 
	if (!::SetCommTimeouts(m_hFile, &cto))	return FALSE;

	::ZeroMemory(&dcb, sizeof(DCB));
	::ZeroMemory(&cto, sizeof(COMMTIMEOUTS));
	//End Config

	return TRUE;
}

BOOL CSerialPort::Close()
{
	DWORD dwR=0;

	do
	{
		SetEvent(m_hEventCloseThread);
	} while (m_bThreadAlive);

	//::CloseHandle(m_hEventCloseThread);
	//m_hEventCloseThread = NULL;
	if (m_hFile == NULL)	return TRUE;
	::CloseHandle(m_hFile);
	m_hFile = NULL;
	return TRUE;
}

UINT CSerialPort::CommThread(LPVOID pParam)
{
	CSerialPort *port = (CSerialPort*)pParam;

	OVERLAPPED olRead;
	::ZeroMemory(&olRead, sizeof(OVERLAPPED));
	olRead.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	if(olRead.hEvent==NULL)	return (UINT)-1;

	port->m_bThreadAlive = TRUE;
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
	DWORD length=0;


	HANDLE hEventArray[2];
	hEventArray[0] = port->m_hEventCloseThread;
	hEventArray[1] = olRead.hEvent;	

	if (port->m_hFile)		// check if the port is opened
		PurgeComm(port->m_hFile, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	BOOL bError=0;
	while(port->m_bThreadAlive)
	{
		bResult = WaitCommEvent(port->m_hFile, &Event, &olRead);
		if(!bResult)
		{
			if(GetLastError()!=ERROR_IO_PENDING)	bError=1;
		}
		else
		{
			bResult = ClearCommError(port->m_hFile, &dwError, &comstat);
			if (comstat.cbInQue == 0) continue;
		}
		if(bError)	break;

		Event = WaitForMultipleObjects(2, hEventArray, FALSE, INFINITE);
		switch (Event)
		{
		case 0:
			port->m_bThreadAlive = FALSE;
			
			//::ZeroMemory(&comstat, sizeof(COMSTAT));
			//CloseHandle(hEventArray[0]);
			//CloseHandle(hEventArray[1]);

			break;
		case 1:	//读数据
			GetCommMask(port->m_hFile, &CommEvent);				
			if (CommEvent & EV_RXCHAR)	port->ReadData(&olRead);

			//CloseHandle(olRead.hEvent);

			break;
		default:
			bError=1;
			break;
		}

		if(bError)	break;
	}

	::ZeroMemory(&comstat, sizeof(COMSTAT));
	CloseHandle(hEventArray[0]);
	CloseHandle(hEventArray[1]);

	return 0;
}

//
// start comm watching
//
BOOL CSerialPort::StartMonitoring()
{
	if (m_hFile == NULL)	return FALSE;

	//LOG4CPLUS_INFO(root, _T("Thread started"));

	// TODO: Add your control notification handler code here
	hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)CommThread,
		this/*NULL*/,
		0,
		&ThreadID);

	return TRUE;
}


//
// Suspend the comm thread
//
BOOL CSerialPort::StopMonitoring()
{
	//LOG4CPLUS_INFO(root, _T("Thread suspended"));

	m_bThreadAlive = FALSE;

	return TRUE;	
}

DWORD CSerialPort::ReadData(OVERLAPPED* olRead)
{
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD dwBytesRead = 0;
	COMSTAT comstat;

	bResult = ClearCommError(m_hFile, &dwError, &comstat);
	if (comstat.cbInQue == 0)	return 0;

	bResult = ReadFile(m_hFile, m_szReadBuffer, m_nReadBufferSize, &dwBytesRead, olRead); 
	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)
			GetOverlappedResult(m_hFile,olRead,&dwBytesRead,1);
		else	
			return 0;
	}
	m_nReadPosition = dwBytesRead;
	//::PostMessage(m_pWndReceMsg->m_hWnd, WM_SCC_MSG, (WPARAM)cpRXBuffer, MAKELPARAM(m_nCOMNo,wBytesRead));
	::ZeroMemory(&comstat, sizeof(COMSTAT));
	return dwBytesRead;
}

DWORD CSerialPort::ReadFormPort(BYTE* lpBuffer){
	DWORD Len = m_nReadPosition;
	if (m_nReadPosition>0)
	{
		memcpy(lpBuffer, m_szReadBuffer, m_nReadPosition);
		//int x = m_nReadPosition - 1;
		//for (int i = 0; i<m_nReadPosition; i++)
		//{
		//	lpBuffer[i] = m_szReadBuffer[x - i];
		//}
		memset(m_szReadBuffer, 0, m_nReadBufferSize);
		m_nReadPosition = 0;
	}
	return Len;
}

DWORD CSerialPort::WriteToPort(LPVOID lpBuffer, DWORD dwLength)
{
	if(!dwLength)	return 0;
	BOOL bResult=TRUE;
	DWORD length=dwLength;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	OVERLAPPED olWrite;

	::ZeroMemory(&olWrite, sizeof(OVERLAPPED));
	olWrite.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	if(olWrite.hEvent==NULL)	return 0;
	ClearCommError(m_hFile, &dwErrorFlags, &ComStat);
	bResult=WriteFile(m_hFile, lpBuffer, length, &length, &olWrite);

	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)
			GetOverlappedResult(m_hFile,&olWrite,&length,TRUE);// 等待
		else	length=0;

	}
	::ZeroMemory(&ComStat, sizeof(COMSTAT));
	::CloseHandle (olWrite.hEvent);

	return length;
}

DWORD CSerialPort::WriteToPort(CString str)
{
	DWORD length = str.GetLength();

	BYTE* m_szWriteBuffer = (BYTE*)malloc(length);;

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));

	memcpy(m_szWriteBuffer, str.GetBuffer(str.GetLength()), length);

	DWORD dw =  WriteToPort(m_szWriteBuffer, length);

	free(m_szWriteBuffer);

	return dw;
}