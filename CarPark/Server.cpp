#include "stdafx.h"
#include "Server.h"

static SOCKET g_sCastSend;
static SOCKET g_sTCPServer;
static CArray<LED_INFO *, LED_INFO *&> g_LedInfos;	//Led信息结构体

CServer::CServer(void)
{
}

CServer::~CServer(void)
{
	for (int i =0; i < g_LedInfos.GetCount(); i++)
	{
		delete g_LedInfos.GetAt(i);
	}
	g_LedInfos.RemoveAll();

	//CloseSocket();
}

void CServer::LoadLedData()
{
	_RecordsetPtr rs = g_DB.GetRecordset(_T("SELECT * FROM LEDInfo"));
	while(!rs->adoEOF)
	{
		LED_INFO *pLedInfo = new LED_INFO;
		CString sTemp;
		if (rs->GetCollect(_T("Address")).vt != VT_NULL)
		{
			pLedInfo->sAddress = (LPCSTR)(_bstr_t)rs->GetCollect(_T("Address"));
		}

		if (rs->GetCollect(_T("Direction")).vt != VT_NULL)
		{
			pLedInfo->sDirection = (LPCSTR)(_bstr_t)rs->GetCollect(_T("Direction"));
		}

		if (rs->GetCollect(_T("ParkingLotNum")).vt != VT_NULL)
		{
			pLedInfo->uParkingLotNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("ParkingLotNum")));
		}

		if (rs->GetCollect(_T("TextFormat")).vt != VT_NULL)
		{
			pLedInfo->sTextRgb = (LPCSTR)(_bstr_t)rs->GetCollect(_T("TextFormat"));
			
		}

		if (rs->GetCollect(_T("TagFormat")).vt != VT_NULL)
		{
			pLedInfo->sTagRgb = (LPCSTR)(_bstr_t)rs->GetCollect(_T("TagFormat"));
		}

		if (rs->GetCollect(_T("ComNum")).vt != VT_NULL)
		{
			pLedInfo->nComNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("ComNum")));
		}

		g_LedInfos.Add(pLedInfo);
		rs->MoveNext();
	}
	rs->Close();
}


// 关闭socket库
bool CServer::CloseSocket()
{
	// 关闭套接字
	::closesocket(g_sCastSend);
	::closesocket(g_sTCPServer);
	return true;
}

// 初始化服务器
bool CServer::InitServer()
{
	// 初始化UDP socket套接字
	if (SOCKET_ERROR == (g_sCastSend = ::socket(AF_INET, SOCK_DGRAM, /*IPPROTO_TCP*/0)))
	{
		AfxMessageBox("Init Socket Error!\n");
		return false;
	}

	// 绑定socket到一个本地地址
	sockaddr_in Castadd;
	Castadd.sin_family = AF_INET;
	Castadd.sin_port = htons(SERVER_PORT_CAST);
	Castadd.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(g_sCastSend, (LPSOCKADDR)&Castadd, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		AfxMessageBox("Bind Error!\n");
		return false;
	}

	//设置socket的属性为广播 
	bool optval=true; 
	setsockopt(g_sCastSend, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(bool));

	//加载LED数据
	LoadLedData();

	//启动广播线程
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BroadcastThread, this, 0, NULL);

	//启动服务器线程
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartServerThread, this, 0, NULL);

	return true;
}

void CServer::BroadcastThread(LPVOID pParam)
{
	int i;
	LED_INFO *pLedInfo;
	CString str1, str2;
	CString sSql;
	int nRedNum = 0, nTem;
	while(g_bAppRun)
	{
		// 把数据广播出去
		CCSDef::TMSG_LEDINFO tMsgLedInfo;
		for (i = 0; i < g_LedInfos.GetCount(); i++)
		{
			pLedInfo = g_LedInfos.GetAt(i);

			nRedNum = 0;
			sSql.Format(_T("SELECT RedLightNum FROM ClientInfo WHERE ClientInfo.LedAddress = '%s'"), pLedInfo->sAddress);
			_RecordsetPtr rs = g_DB.GetRecordset((_bstr_t)sSql);
			while(!rs->adoEOF)
			{
				if (rs->GetCollect(_T("RedLightNum")).vt != VT_NULL)
				{
					nTem = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RedLightNum")));
					if (nTem > 0)
					{
						nRedNum += nTem;
					}
				}
				rs->MoveNext();
			}
			rs->Close();

			nTem = pLedInfo->uParkingLotNum - nRedNum;

			str1.Format(_T("%s@%s@%d@%s@%s@%d"), pLedInfo->sAddress, pLedInfo->sDirection,
				nTem > 0 ? nTem : 0, pLedInfo->sTextRgb, pLedInfo->sTagRgb, pLedInfo->nComNum);
			if (i == 0)
			{
				str2 = str1;
			}else
			{
				str2 += _T("$") + str1;
			}
		}
		strcpy(tMsgLedInfo.szBuff, str2);

		SOCKADDR_IN dstAdd; 
		dstAdd.sin_family = AF_INET; 
		dstAdd.sin_port = htons(CAST_PORT);
		dstAdd.sin_addr.s_addr = INADDR_BROADCAST;
		if(SOCKET_ERROR == ::sendto(g_sCastSend, (char*)(&tMsgLedInfo), sizeof(CCSDef::TMSG_DEVICEINFO), 0, (SOCKADDR*)&dstAdd, sizeof(SOCKADDR)))
		{
			TRACE("Send Data Error!\n");
			return;
		}
		Sleep(400);
	}
}

///////////////////////////////////////////////////////////////////
//////////////TCP   服务端
///////////////////////////////////////////////////////////////////
void CServer::StartServerThread(LPVOID pParam)
{
	// 创建TCP socket套接字
	if (SOCKET_ERROR == (g_sTCPServer = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox("Init Socket Error!\n");
		return;
	}

	// 绑定socket到一个本地地址
	sockaddr_in TCPadd;
	TCPadd.sin_family = AF_INET;
	TCPadd.sin_port = htons(SERVER_PORT_TCP);
	TCPadd.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(g_sTCPServer, (LPSOCKADDR)&TCPadd, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		AfxMessageBox("Bind Error!\n");
		return;
	}

	// 设置socket进入监听状态
	if (::listen(g_sTCPServer, SOMAXCONN) == SOCKET_ERROR)
	{
		AfxMessageBox("Listen Error!\n");
		return;
	}

	SOCKET sClient;

	while(g_bAppRun)
	{
		sockaddr_in ClientAddr;
		int nAddrLen = sizeof(sockaddr_in);

		sClient = ::accept(g_sTCPServer, (sockaddr*)&ClientAddr, &nAddrLen);
		if ( sClient == INVALID_SOCKET )
		{
			break;
		}

		//启动服务器接收线程
		::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)STCPRecvThread, (LPVOID)sClient, 0, NULL);
	}
	::closesocket(sClient);
}

void CServer::STCPRecvThread(LPVOID pParam)
{
	SOCKET sClient = (SOCKET)pParam;

	// 循环接收client端的连接请求
	long g_lLength;
	char g_szFileName[MAXFILEDIRLENGTH];
	char g_szBuff[MAX_PACKET_SIZE + 1];

	while(g_bAppRun)
	{
		memset(g_szBuff, 0, sizeof(g_szBuff));
		int nRecv = ::recv(sClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);
		if (nRecv == 0 || nRecv == SOCKET_ERROR)
		{
			TCHAR("客服端关闭");
			break;
		}

		if (nRecv > 0)
		{
			g_szBuff[nRecv] = '\0';
		}

		// 解析命令
		CCSDef::TMSG_HEADER* pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;

		switch (pMsgHeader->cMsgID)
		{
		case MSG_LEDINFO: 
			{
				//OpenFileError(pMsgHeader);
			}
			break;
		case MSG_DEVICEINFO:
			{
				CCSDef::TMSG_DEVICEINFO* pDeviceInfoMsg = (CCSDef::TMSG_DEVICEINFO*)pMsgHeader;
				CString sSql;
				sSql.Format(_T("UPDATE ClientInfo SET RedLightNum = %d, GreenLightNum = %d, NoneLightNum = %d WHERE ClientID = '%s'"),
					pDeviceInfoMsg->nRedLightNum, pDeviceInfoMsg->nGreenLightNum, pDeviceInfoMsg->nNoneLightNum, pDeviceInfoMsg->szDeviceName);
				g_DB.ExecuteSQL((_bstr_t)sSql);
			}
			break;
		}
	}
}

