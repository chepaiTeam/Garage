#include "stdafx.h"
#include "Client.h"
#include "Server.h"

static SOCKET g_sCastRecv;
static SOCKET g_sTCPClient;
static char g_sServerIP[256];

CClient::CClient()
{
	memset(g_sServerIP, 0, sizeof(g_sServerIP));
}

CClient::~CClient()
{
	CloseSocket();
}

// �ر�socket��
bool CClient::CloseSocket()
{
	// �ر��׽���
	::closesocket(g_sCastRecv);
	::closesocket(g_sTCPClient);
	return true;
}

void CClient::InitClient()
{
	// ����socket�׽���
	if (SOCKET_ERROR == (g_sCastRecv = ::socket(AF_INET, SOCK_DGRAM, /*IPPROTO_TCP*/0)))
	{
		AfxMessageBox("Init Socket Error!\n");
		return;
	}

	bool optval=true; 
	setsockopt(g_sCastRecv, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));

	// ��socket��һ�����ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(CAST_PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(g_sCastRecv, (LPSOCKADDR)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		AfxMessageBox("Bind Error!\n");
		return;
	}

	//���������߳�
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CCastRecvThread, NULL, 0, NULL);
}

void CClient::StartClientThread(LPVOID pParam)
{
	// ��ʼ��socket�׽���
	if (SOCKET_ERROR == (g_sTCPClient = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox("Init Socket Error!\n");
		return;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERVER_PORT_TCP);
	servAddr.sin_addr.S_un.S_addr = ::inet_addr(g_sServerIP);
	
	if (INVALID_SOCKET == (::connect(g_sTCPClient, (sockaddr*)&servAddr, sizeof(sockaddr_in))))
	{
		TRACE("Connect to Server Error!\n");

		::closesocket(g_sTCPClient);
		//�ٴ������ͷ����߳�
		::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartClientThread, NULL, 0, NULL);
		return;
	}

	//�����ͻ��˽����߳�
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTCPRecvThread, NULL, 0, NULL);

	//�����ͻ��˷����߳�
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTCPSendThread, NULL, 0, NULL);
}

void CClient::CTCPSendThread(LPVOID pParam)
{
	// ���豸��Ϣ����server��
	CCSDef::TMSG_DEVICEINFO tMsgDeviceInfo;
	strcpy(tMsgDeviceInfo.szDeviceName, g_sDeviceID);
	tMsgDeviceInfo.nRedLightNum = -1;
	tMsgDeviceInfo.nGreenLightNum = -1;
	tMsgDeviceInfo.nNoneLightNum = -1;

	int i;
	while(g_bAppRun)
	{
		Sleep(100);
		int nRedNum = 0, nGreenNum = 0, nNoneNum = 0;

		//ͳ�Ƶ�����
		for (i = 0; i < g_pCamInfos.GetCount(); i++)
		{
			switch (g_pCamInfos.GetAt(i)->nLightColor)
			{
			case 0:
				nNoneNum++;
				break;
			case 1:
				nRedNum++;
				break;
			case 2:
				nGreenNum++;
				break;
			}
		}

		if (tMsgDeviceInfo.nNoneLightNum == nNoneNum
			&& tMsgDeviceInfo.nGreenLightNum == nGreenNum
			&& tMsgDeviceInfo.nRedLightNum == nRedNum)
		{
			//continue;
		}
		tMsgDeviceInfo.nNoneLightNum = nNoneNum;
		tMsgDeviceInfo.nGreenLightNum = nGreenNum;
		tMsgDeviceInfo.nRedLightNum = nRedNum;

		if (SOCKET_ERROR == ::send(g_sTCPClient, (char*)(&tMsgDeviceInfo), sizeof(CCSDef::TMSG_DEVICEINFO), 0))
		{
			TRACE("Send Data Error!\n");

			::closesocket(g_sTCPClient);
			//�ٴ������ͷ����߳�
			::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartClientThread, NULL, 0, NULL);
			return;
		}
	}
}

void CClient::CTCPRecvThread(LPVOID pParam)
{
	// ѭ������Server�˵���������
	char g_szBuff[MAX_PACKET_SIZE + 1];

	while(g_bAppRun)
	{
		memset(g_szBuff, 0, sizeof(g_szBuff));
		int nRecv = ::recv(g_sTCPClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);
		if (nRecv == 0 || nRecv == SOCKET_ERROR)
		{
			TRACE("�������ر�");
			break;
		}

		if (nRecv > 0)
		{
			g_szBuff[nRecv] = '\0';
		}

		// ��������
		CCSDef::TMSG_HEADER* pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;

		switch (pMsgHeader->cMsgID)
		{
		case MSG_DEVICEINFO:
			break;
		case MSG_LEDINFO:
			{
				CCSDef::TMSG_LEDINFO* pLedMsg = new CCSDef::TMSG_LEDINFO;
				memcpy(pLedMsg, pMsgHeader, sizeof(CCSDef::TMSG_LEDINFO));
				//�����ַ����ӷ��ʹ�������
				::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CJXDataThread, pLedMsg, 0, NULL);
			}
			break;
		}
	}
}

void CClient::CJXDataThread(LPVOID pParam)
{
	CCSDef::TMSG_LEDINFO* pLedMsg = (CCSDef::TMSG_LEDINFO*)pParam;

	LED_INFO LedInfo;
	CString s1, s2, s3, s4;
	int nS1 = 0, nS2 = 0, nS3 = 0, nS4 = 0;
	int nMin, nMax;
	CString sRgb;
	while(AfxExtractSubString(s1, pLedMsg->szBuff, nS1, '$'))
	{
		while(AfxExtractSubString(s2, s1, nS2, '@'))
		{
			switch(nS2)
			{
			case 0:
				LedInfo.sAddress = s2;
				break;
			case 1:
				LedInfo.sDirection = s2;
				break;
			case 2:
				LedInfo.uParkingLotNum = _ttoi(s2);
				break;
			case 3:
				{
					nS3 = 0;
					nS4 = 0;
					while(AfxExtractSubString(s3, s2, nS3, ';'))
					{
						while(AfxExtractSubString(s4, s3, nS4, ':'))
						{
							switch(nS4)
							{
							case 0:
								s4 == "" ? nMin = -1 : nMin = _ttoi(s4);
								break;
							case 1:
								s4 == "" ? nMax = -1 : nMax = _ttoi(s4);
								break;
							case 2:
								sRgb = s4;
								break;
							}
							nS4++;
						}
						if (nS4 == 3)
						{
							RGB_INFO *RgbInfo = new RGB_INFO;
							RgbInfo->nMin = nMin;
							RgbInfo->nMax = nMax;
							RgbInfo->sRgb = sRgb;
							LedInfo.TextRgbs.Add(RgbInfo);
						}
						nS4 = 0;
						nS3++;
					}
				}
				break;
			case 4:
				{
					nS3 = 0;
					nS4 = 0;
					while(AfxExtractSubString(s3, s2, nS3, ';'))
					{
						while(AfxExtractSubString(s4, s3, nS4, ':'))
						{
							switch(nS4)
							{
							case 0:
								s4 == "" ? nMin = -1 : nMin = _ttoi(s4);
								break;
							case 1:
								s4 == "" ? nMax = -1 : nMax = _ttoi(s4);
								break;
							case 2:
								sRgb = s4;
								break;
							}
							nS4++;
						}
						if (nS4 == 3)
						{
							RGB_INFO *RgbInfo = new RGB_INFO;
							RgbInfo->nMin = nMin;
							RgbInfo->nMax = nMax;
							RgbInfo->sRgb = sRgb;
							LedInfo.TagRgbs.Add(RgbInfo);
						}
						nS4 = 0;
						nS3++;
					}
				}
				break;
			case 5:
				LedInfo.nComNum = _ttoi(s2);
				break;
			}
			nS2++;
		}
		if (nS2 == 6)
		{
			//��LED��Ӧ�Ĵ��ڷ�������
			//�������������ı���ɫ
			CString sTextRgb = _T("R");
			for (int i = 0; i < LedInfo.TextRgbs.GetCount(); i++)
			{
				if (LedInfo.TextRgbs[i]->nMin >= 0 && LedInfo.TextRgbs[i]->nMax >= 0)
				{
					if (LedInfo.TextRgbs[i]->nMin <= LedInfo.uParkingLotNum
						&& LedInfo.TextRgbs[i]->nMax > LedInfo.uParkingLotNum)
					{
						sTextRgb = LedInfo.TextRgbs[i]->sRgb;
					}
				}else if (LedInfo.TextRgbs[i]->nMin < 0 && LedInfo.TextRgbs[i]->nMax >= 0)
				{
					if (LedInfo.TextRgbs[i]->nMax > LedInfo.uParkingLotNum)
					{
						sTextRgb = LedInfo.TextRgbs[i]->sRgb;
					}
				}else if (LedInfo.TextRgbs[i]->nMin >= 0 && LedInfo.TextRgbs[i]->nMax < 0)
				{
					if (LedInfo.TextRgbs[i]->nMin <= LedInfo.uParkingLotNum)
					{
						sTextRgb = LedInfo.TextRgbs[i]->sRgb;
					}
				}else
				{
					sTextRgb = LedInfo.TextRgbs[i]->sRgb;
				}
			}

			//�������������ı���ɫ
			CString sTagRgb = _T("R");
			for (int i = 0; i < LedInfo.TagRgbs.GetCount(); i++)
			{
				if (LedInfo.TagRgbs[i]->nMin >= 0 && LedInfo.TagRgbs[i]->nMax >= 0)
				{
					if (LedInfo.TagRgbs[i]->nMin <= LedInfo.uParkingLotNum
						&& LedInfo.TagRgbs[i]->nMax > LedInfo.uParkingLotNum)
					{
						sTagRgb = LedInfo.TagRgbs[i]->sRgb;
					}
				}else if (LedInfo.TagRgbs[i]->nMin < 0 && LedInfo.TagRgbs[i]->nMax >= 0)
				{
					if (LedInfo.TagRgbs[i]->nMax > LedInfo.uParkingLotNum)
					{
						sTagRgb = LedInfo.TagRgbs[i]->sRgb;
					}
				}else if (LedInfo.TagRgbs[i]->nMin >= 0 && LedInfo.TagRgbs[i]->nMax < 0)
				{
					if (LedInfo.TagRgbs[i]->nMin <= LedInfo.uParkingLotNum)
					{
						sTagRgb = LedInfo.TagRgbs[i]->sRgb;
					}
				}else
				{
					sTagRgb = LedInfo.TagRgbs[i]->sRgb;
				}
			}
			CString sLedData;
			sLedData.Format(_T("/P%s%s%s%s%03d>"), LedInfo.sAddress, LedInfo.sDirection, sTagRgb, sTextRgb, LedInfo.uParkingLotNum);
			if (LedInfo.nComNum == 1)
			{
				g_pPort1->WriteToPort(sLedData);
			}else if (LedInfo.nComNum == 2)
			{
				g_pPort2->WriteToPort(sLedData);
			}
		}
		nS2 = 0;
		nS1++;

		for (int i = 0; i < LedInfo.TextRgbs.GetCount(); i++)
		{
			delete LedInfo.TextRgbs[i];
		}
		LedInfo.TextRgbs.RemoveAll();

		for (int i = 0; i < LedInfo.TagRgbs.GetCount(); i++)
		{
			delete LedInfo.TagRgbs[i];
		}
		LedInfo.TagRgbs.RemoveAll();
	}

	delete pLedMsg;
	pLedMsg = NULL;
}

void CClient::CCastRecvThread(LPVOID pParam)
{
	long g_lLength;
	char* g_pBuff;
	char g_szFileName[MAXFILEDIRLENGTH];
	char g_szBuff[MAX_PACKET_SIZE + 1];

	SOCKADDR_IN sender;
	int senferAddSize = sizeof(sender);
	while(g_bAppRun)
	{
		int r = ::recvfrom(g_sCastRecv, g_szBuff, MAX_PACKET_SIZE + 1, 0, (SOCKADDR*)&sender, &senferAddSize);
		if(SOCKET_ERROR == r || r < 0)
		{
			Sleep(300);
			TRACE("Recv Data Error!\n");
			continue;
		}

		if(strcmp(g_sServerIP, inet_ntoa(sender.sin_addr)))
		{
			strcpy(g_sServerIP, inet_ntoa(sender.sin_addr));
			//����ȡ��������IPʱ�����ͷ����߳�
			::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartClientThread, NULL, 0, NULL);
		}

		CCSDef::TMSG_HEADER *pMsgHeader;
		pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;

		switch (pMsgHeader->cMsgID)
		{
		case MSG_DEVICEINFO:
			break;
		case MSG_LEDINFO:
			{
				CCSDef::TMSG_LEDINFO* pLedMsg = new CCSDef::TMSG_LEDINFO;
				memcpy(pLedMsg, pMsgHeader, sizeof(CCSDef::TMSG_LEDINFO));
				//�����ַ����ӷ��ʹ�������
				::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CJXDataThread, pLedMsg, 0, NULL);
			}
			break;
		}
	}
}