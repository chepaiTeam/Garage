#include "stdafx.h"
#include "Server.h"


CServer::CServer()
{
	g_pBuff = NULL;

	ListenToClient();
	CloseSocket();
}

CServer::~CServer()
{

}

void CServer::ListenToClient()
{
	// ����socket�׽���
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sListen)
	{
		AfxMessageBox("Init Socket Error!\n");
		return;
	}

	// ��socket��һ�����ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(sListen, (LPSOCKADDR)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		AfxMessageBox("Bind Error!\n");
		return;
	}

	// ����socket�������״̬
	if (::listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		AfxMessageBox("Listen Error!\n");
		return;
	}

	TRACE("Listening To Client...\n");

	// ѭ������client�˵���������
	sockaddr_in ClientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET sClient;

	while (INVALID_SOCKET == (sClient = ::accept(sListen, (sockaddr*)&ClientAddr, &nAddrLen)))
	{
	}

	while (true == ProcessMsg(sClient))
	{
	}

	// �ر�ͬ�ͻ��˵�����
	::closesocket(sClient);

	::closesocket(sListen);
}

bool CServer::CloseSocket()
{
	if (NULL != g_pBuff)
	{
		delete [] g_pBuff;
		g_pBuff = NULL;
	}

	return true;
}

bool CServer::ProcessMsg(SOCKET sClient)
{
	int nRecv = ::recv(sClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);
	if (nRecv > 0)
	{
		g_szBuff[nRecv] = '\0';
	}

	// ��������
	CCSDef::TMSG_HEADER* pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;
	switch (pMsgHeader->cMsgID)
	{
	case MSG_FILENAME:    // �ļ���
		{
			OpenFile(pMsgHeader, sClient);
		}
		break;
	case MSG_CLIENT_READY:   // �ͻ���׼������,��ʼ�����ļ�
		{
			SendFile(sClient);
		}
		break;
	case MSG_SENDFILESUCCESS: // �����ļ��ɹ�
		{
			TRACE("Send File Success!\n");
			return false;
		}
		break;
	case MSG_FILEALREADYEXIT_ERROR: // Ҫ������ļ��Ѿ�������
		{
			TRACE("The file reay to send already exit!\n");
			return false;
		}
		break;
	}

	return true;
}

bool CServer::ReadFile(SOCKET sClient)
{
	if (NULL != g_pBuff)
	{
		return true;
	}

	// ���ļ�
	FILE *pFile;
	if (NULL == (pFile = fopen(g_szNewFileName, "rb")))   // ���ļ�ʧ��
	{
		TRACE("Cannot find the file, request the client input file name again\n");
		CCSDef::TMSG_ERROR_MSG tMsgErrorMsg(MSG_OPENFILE_ERROR);
		::send(sClient, (char*)(&tMsgErrorMsg), sizeof(CCSDef::TMSG_ERROR_MSG), 0);
		return false;
	}

	// ���ļ��ĳ��ȴ��ص�clientȥ
	fseek(pFile, 0, SEEK_END);
	g_lLength = ftell(pFile);
	TRACE("File Length = %d\n", g_lLength);
	CCSDef::TMSG_FILELENGTH tMsgFileLength(g_lLength);
	::send(sClient, (char*)(&tMsgFileLength), sizeof(CCSDef::TMSG_FILELENGTH), 0);

	// �����ļ�ȫ·����,���ļ����ֽ����
	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFname[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(g_szNewFileName, szDrive, szDir, szFname, szExt);
	strcat(szFname,szExt);
	CCSDef::TMSG_FILENAME tMsgFileName;
	strcpy(tMsgFileName.szFileName, szFname);
	TRACE("Send File Name: %s\n", tMsgFileName.szFileName);
	::send(sClient, (char*)(&tMsgFileName), sizeof(CCSDef::TMSG_FILENAME), 0);

	// ���仺������ȡ�ļ�����
	g_pBuff = new char[g_lLength + 1];
	if (NULL == g_pBuff)
	{
		return false;
	}

	fseek(pFile, 0, SEEK_SET);
	fread(g_pBuff, sizeof(char), g_lLength, pFile);
	g_pBuff[g_lLength] = '\0';
	fclose(pFile);

	return true;
}

// ���ļ�
bool CServer::OpenFile(CCSDef::TMSG_HEADER* pMsgHeader, SOCKET sClient)
{
	CCSDef::TMSG_FILENAME* pRequestFilenameMsg = (CCSDef::TMSG_FILENAME*)pMsgHeader;

	// ���ļ�·��������һЩ����
	char *p1, *p2;
	for (p1 = pRequestFilenameMsg->szFileName, p2 = g_szNewFileName;
		'\0' != *p1;
		++p1, ++p2)
	{
		if ('\n' != *p1)
		{
			*p2 = *p1;
		}
		if ('\\' == *p2)
		{
			*(++p2) = '\\';
		}
	}
	*p2 = '\0';

	ReadFile(sClient);

	return true;
}

// �����ļ�
bool CServer::SendFile(SOCKET sClient)
{
	if (NULL == g_pBuff)
	{
		ReadFile(sClient);
	}

	int nPacketBufferSize = MAX_PACKET_SIZE - 2 * sizeof(int); // ÿ�����ݰ�����ļ���buffer��С
	// ����ļ��ĳ��ȴ���ÿ�����ݰ����ܴ��͵�buffer������ô�ͷֿ鴫��
	for (int i = 0; i < g_lLength; i += nPacketBufferSize)
	{  
		CCSDef::TMSG_FILE tMsgFile;
		tMsgFile.tFile.nStart = i;
		if (i + nPacketBufferSize + 1> g_lLength)
		{
			tMsgFile.tFile.nSize = g_lLength - i;
		}
		else
		{
			tMsgFile.tFile.nSize = nPacketBufferSize;
		}
		//AfxMessageBox("start = %d, size = %d\n", tMsgFile.tFile.nStart, tMsgFile.tFile.nSize);
		memcpy(tMsgFile.tFile.szBuff, g_pBuff + tMsgFile.tFile.nStart, tMsgFile.tFile.nSize);
		::send(sClient, (char*)(&tMsgFile), sizeof(CCSDef::TMSG_FILE), 0);
		Sleep(0.5);
	}

	delete [] g_pBuff;
	g_pBuff = NULL;

	return true;
}