#include "stdafx.h"
#include "Server.h"

static SOCKET g_sCastSend;
static SOCKET g_sTCPServer;
static CArray<LED_INFO *, LED_INFO *&> g_LedInfos;	//Led信息结构体
//static CArray<GROUP_INFO *, GROUP_INFO *&> g_GroupInfos;	//组信息结构体
static CArray<DEVICE_INFO *, DEVICE_INFO *&> g_DeviceInfos;	//设备信息结构体

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

	//for (int i =0; i < g_GroupInfos.GetCount(); i++)
	//{
	//	delete g_GroupInfos.GetAt(i);
	//}
	//g_GroupInfos.RemoveAll();
	//CloseSocket();
}

void CServer::LoadData()
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

	//rs = g_DB.GetRecordset(_T("SELECT * FROM GroupInfo"));
	//while(!rs->adoEOF)
	//{
	//	GROUP_INFO *pGroupInfo = new GROUP_INFO;
	//	CString sTemp;
	//	if (rs->GetCollect(_T("GroupID")).vt != VT_NULL)
	//	{
	//		pGroupInfo->sGroupID = (LPCSTR)(_bstr_t)rs->GetCollect(_T("GroupID"));
	//	}

	//	if (rs->GetCollect(_T("LedAddress")).vt != VT_NULL)
	//	{
	//		pGroupInfo->sLedAddress = (LPCSTR)(_bstr_t)rs->GetCollect(_T("LedAddress"));
	//	}

	//	if (rs->GetCollect(_T("Effective")).vt != VT_NULL)
	//	{
	//		pGroupInfo->nEffective = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("Effective")));
	//	}

	//	if (rs->GetCollect(_T("RedLightNum")).vt != VT_NULL)
	//	{
	//		pGroupInfo->nRedLightNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RedLightNum")));

	//	}

	//	if (rs->GetCollect(_T("GreenLightNum")).vt != VT_NULL)
	//	{
	//		pGroupInfo->nGreenLightNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("GreenLightNum")));
	//	}

	//	if (rs->GetCollect(_T("RGLightNum")).vt != VT_NULL)
	//	{
	//		pGroupInfo->nRGLightNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RGLightNum")));
	//	}

	//	if (rs->GetCollect(_T("NoneLightNum")).vt != VT_NULL)
	//	{
	//		pGroupInfo->nNoneLightNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("NoneLightNum")));
	//	}

	//	g_GroupInfos.Add(pGroupInfo);
	//	rs->MoveNext();
	//}
	//rs->Close();
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
	LoadData();

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
			sSql.Format(_T("SELECT SUM(RedLightNum) AS RedLightNum1 FROM GroupInfo WHERE LedAddress = '%s'"), pLedInfo->sAddress);
			_RecordsetPtr rs = g_DB.GetRecordset((_bstr_t)sSql);
			while(!rs->adoEOF)
			{
				if (rs->GetCollect(_T("RedLightNum1")).vt != VT_NULL)
				{
					nTem = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RedLightNum1")));
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
		if(SOCKET_ERROR == ::sendto(g_sCastSend, (char*)(&tMsgLedInfo), sizeof(CCSDef::TMSG_GROUPINFO), 0, (SOCKADDR*)&dstAdd, sizeof(SOCKADDR)))
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
struct clientdata
{
	sockaddr_in ClientAddr;
	SOCKET sClient;
};

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

	while(g_bAppRun)
	{
		int nAddrLen = sizeof(sockaddr_in);

		clientdata *pcdata = new clientdata;
		pcdata->sClient = ::accept(g_sTCPServer, (sockaddr*)&pcdata->ClientAddr, &nAddrLen);
		if ( pcdata->sClient == INVALID_SOCKET )
		{
			break;
		}

		//启动服务器接收线程
		::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)STCPRecvThread, (LPVOID)pcdata, 0, NULL);
	}
}

void CServer::STCPRecvThread(LPVOID pParam)
{
	clientdata *pcdata = (clientdata *)pParam;
	// 循环接收client端的连接请求
	char g_szBuff[MAX_PACKET_SIZE + 1];
	int nRedLightNum;
	int nGreenLightNum;
	int nRGLightNum;
	int nNoneLightNum;

	while(g_bAppRun)
	{
		memset(g_szBuff, 0, sizeof(g_szBuff));
		int nRecv = ::recv(pcdata->sClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);
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
				CCSDef::TMSG_GROUPINFO* pGroupInfoMsg = (CCSDef::TMSG_GROUPINFO*)pMsgHeader;

				//添加或修改设备信息
				DEVICE_INFO *DeviceInfo = NULL;
				BOOL bFlag = FALSE;
				for (int i = 0; i < g_DeviceInfos.GetCount(); i++)
				{
					if(g_DeviceInfos.GetAt(i)->sIP == inet_ntoa(pcdata->ClientAddr.sin_addr))
					{
						DeviceInfo = g_DeviceInfos.GetAt(i);
						bFlag = TRUE;
					}
				}
				if (!bFlag)
				{
					DeviceInfo = new DEVICE_INFO;
					DeviceInfo->sIP = inet_ntoa(pcdata->ClientAddr.sin_addr);
					g_DeviceInfos.Add(DeviceInfo);
				}

				//添加或修改组信息
				GROUP_INFO *GroupInfo = NULL;
				bFlag = FALSE;
				for (int i = 0; i < DeviceInfo->aGroupInfos.GetCount(); i++)
				{
					if(DeviceInfo->aGroupInfos.GetAt(i)->sGroupID == pGroupInfoMsg->szGroupID)
					{
						GroupInfo = DeviceInfo->aGroupInfos.GetAt(i);
						bFlag = TRUE;
					}
				}
				if (!bFlag)
				{
					GroupInfo = new GROUP_INFO;
					GroupInfo->sGroupID = pGroupInfoMsg->szGroupID;
					DeviceInfo->aGroupInfos.Add(GroupInfo);
				}
				GroupInfo->nEffective = 1;
				GroupInfo->nGreenLightNum = pGroupInfoMsg->nGreenLightNum;
				GroupInfo->nNoneLightNum = pGroupInfoMsg->nNoneLightNum;
				GroupInfo->nRedLightNum = pGroupInfoMsg->nRedLightNum;
				GroupInfo->nRGLightNum = pGroupInfoMsg->nRGLightNum;

				nRedLightNum = 0;
				nGreenLightNum = 0;
				nRGLightNum = 0;
				nNoneLightNum = 0;
				//遍历设备中与该组相关的信息
				for (int i = 0; i < g_DeviceInfos.GetCount(); i++)
				{
					DeviceInfo = g_DeviceInfos.GetAt(i);
					for (int j = 0; j < DeviceInfo->aGroupInfos.GetCount(); j++)
					{
						GroupInfo = DeviceInfo->aGroupInfos.GetAt(j);
						if (GroupInfo->sGroupID == pGroupInfoMsg->szGroupID)
						{
							nRedLightNum += GroupInfo->nRedLightNum;
							nGreenLightNum += GroupInfo->nGreenLightNum;
							nNoneLightNum += GroupInfo->nNoneLightNum;
							nRGLightNum += GroupInfo->nRGLightNum;
						}
					}
				}

				CString sSql;
				sSql.Format(_T("UPDATE GroupInfo SET RedLightNum = %d, GreenLightNum = %d, NoneLightNum = %d, RGLightNum = %d WHERE GroupID = '%s'"),
					nRedLightNum, nGreenLightNum, nNoneLightNum, nRGLightNum, pGroupInfoMsg->szGroupID);
				g_DB.ExecuteSQL((_bstr_t)sSql);
			}
			break;
		}
	}

	::closesocket(pcdata->sClient);
	delete pcdata;
	pcdata = NULL;
}

