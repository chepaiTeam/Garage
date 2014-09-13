#pragma once
class CSerialPort
{
//public:
//	HANDLE m_hComm;
//
//	HANDLE hThread;
//	DWORD ThreadID;
//
//	BOOL		m_bIsRead;
//	BOOL		m_bIsWrite;
//
//	// start/stop comm watching
//	BOOL		StartMonitoring();
//	BOOL		StopMonitoring();
//
//	DWORD		GetWriteBufferSize();
//	DWORD		GetCommEvents();
//	DCB			GetDCB();
//
//	//void		WriteToPort(char* string);
//	//void		WriteToPort(char* string,int nLen);
//	//void		WriteToPort(CString string);
//	//void		WriteToPort(LPCTSTR string,int nLen);
//	void		WriteToPort(BYTE* nByte, int nLen);
//	DWORD		ReadFormPort(BYTE* nByte, int nLen);
//protected:
//	// protected memberfunctions
//	void		ProcessErrorMessage(CString ErrorText);
//	static UINT	CommThread(LPVOID pParam);
//	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);
//	static void	WriteChar(CSerialPort* port);
//
//	// synchronisation objects
//	CRITICAL_SECTION	m_csCommunicationSync;
//	BOOL				m_bThreadAlive;
//
//	// handles
//	HANDLE				m_hWriteEvent;
//	HANDLE				m_hShutdownEvent;
//
//	// Event array. 
//	// One element is used for each event. There are two event handles for each port.
//	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
//	// There is a general shutdown when the port is closed. 
//	HANDLE				m_hEventArray[3];
//
//	// structures
//	OVERLAPPED			m_ov;
//	COMMTIMEOUTS		m_CommTimeouts;
//	DCB					m_dcb;
//
//	// owner window
//	//CWnd*				m_pOwner;
//
//	// misc
//	LPSTR				m_nPortName;
//	DWORD				m_dwCommEvents;
//
//	char*				m_szReadBuffer;
//	DWORD				m_nReadPosition;
//	DWORD				m_nReadBufferSize;
//
//	char*				m_szWriteBuffer;
//	DWORD				m_nWriteSize;
//	DWORD				m_nWriteBufferSize;
//
//
//	//DWORD				m_nReadBufferLen;		//读取缓冲区当前数据长度，读取清零。
//
//public:
//	CSerialPort(void);
//	virtual ~CSerialPort(void);
//	
//	BOOL InitSerialPort(CString PortName = "COM1", UINT BaudRate = 19200, char Parity = 'N', UINT Databits = 8, UINT Stopbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT ReadWriteBufferSize = 1024);
//	void ClosePort(void);

public:
	CSerialPort();
	virtual ~CSerialPort();


	// start/stop comm watching
	BOOL StartMonitoring();
	BOOL StopMonitoring();

	BOOL Open(CString PortName = "COM1", UINT BaudRate = 19200, char Parity = 'N', UINT Databits = 8, UINT Stopbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT ReadWriteBufferSize = 1024);
	BOOL IsOpen (void) const	{ return (m_hFile != NULL); }
	BOOL Close();

	DWORD WriteToPort(LPVOID lpBuffer, DWORD dwLength);
	DWORD WriteToPort(CString str);
	DWORD ReadFormPort(BYTE* lpBuffer);
protected:
	HANDLE	m_hFile;				//串口句柄
	HANDLE hThread;
	DWORD ThreadID;

	char* m_szReadBuffer;
	DWORD m_nReadPosition;
	DWORD m_nReadBufferSize;

	HANDLE m_hEventCloseThread;		//关闭串口辅助线程
	BOOL m_bThreadAlive;			//串口辅助线程活动标志

	DWORD ReadData(OVERLAPPED* olRead);
	static UINT CommThread(LPVOID pParam);
};

