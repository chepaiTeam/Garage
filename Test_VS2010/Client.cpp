#include "stdafx.h"
#include "Client.h"


CClient::CClient(void)
{
	g_lLength = 0;
	g_pBuff = NULL;
}


CClient::~CClient(void)
{
}


// �ر�socket��
bool CClient::CloseSocket()
{
	// �ر��׽���
	::closesocket(g_sClient);
	return true;
}

// ��server�����ӽ����ļ��Ĵ���
bool CClient::ConectToServer()
{
	// ��ʼ��socket�׽���
	if (SOCKET_ERROR == (g_sClient = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox("Init Socket Error!\n");
		return false;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.S_un.S_addr = ::inet_addr(SERVER_IP);
	if (INVALID_SOCKET == (::connect(g_sClient, (sockaddr*)&servAddr, sizeof(sockaddr_in))))
	{
		AfxMessageBox("Connect to Server Error!\n");
		return false;
	}

	// �����ļ�·�����䵽server��
	SendFileNameToServer();

	// ����server�˴���������Ϣ,ֱ�������ļ��ɹ�Ϊֹ
	while (true == ProcessMsg())
	{
	}

	return true;
}

// ���û�������ļ�·�����͵�server��
bool CClient::SendFileNameToServer()
{
	char szFileName[MAXFILEDIRLENGTH];
	TRACE("Input the File Directory: ");

	fgets(szFileName, MAXFILEDIRLENGTH, stdin);

	// ���ļ�·������server��
	CCSDef::TMSG_FILENAME tMsgRequestFileName;
	strcpy(tMsgRequestFileName.szFileName, szFileName);
	if (SOCKET_ERROR == ::send(g_sClient, (char*)(&tMsgRequestFileName), sizeof(CCSDef::TMSG_FILENAME), 0))
	{
		TRACE("Send File Name Error!\n");
		return false;
	}

	return true;
}

// ����server�˴��͹�������Ϣ
bool CClient::ProcessMsg()
{
	CCSDef::TMSG_HEADER *pMsgHeader;
	int nRecv = ::recv(g_sClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);

	pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;

	switch (pMsgHeader->cMsgID)
	{
	case MSG_OPENFILE_ERROR:   // ���ļ�����
		{
			OpenFileError(pMsgHeader);
		}
		break;
	case MSG_FILELENGTH:    // �ļ��ĳ���
		{
			if (0 == g_lLength)
			{
				g_lLength = ((CCSDef::TMSG_FILELENGTH*)pMsgHeader)->lLength;
				TRACE("File Length: %d\n", g_lLength);
			}
		}
		break;
	case MSG_FILENAME:     // �ļ���
		{
			return AllocateMemoryForFile(pMsgHeader);
		}
		break;
	case MSG_FILE:      // �����ļ�,д���ļ��ɹ�֮���˳��������
		{
			if (WriteToFile(pMsgHeader))
			{
				return false;
			}
		}
		break;
	}

	return true;
}

// ���ļ�ʧ��
bool CClient::OpenFileError(CCSDef::TMSG_HEADER *pMsgHeader)
{
	if (NULL != g_pBuff)
		return true;
	assert(NULL != pMsgHeader);

	TRACE("Cannot find file!Please input again!\n");

	// ���������ļ�����
	SendFileNameToServer();

	return true;
}

// �����Ƿ��Ѿ�������Ҫ������ļ�,ͬʱ���仺���������ļ�
bool CClient::AllocateMemoryForFile(CCSDef::TMSG_HEADER *pMsgHeader)
{
	assert(NULL != pMsgHeader);

	if (NULL != g_pBuff)
	{
		return true;
	}

	CCSDef::TMSG_FILENAME* pRequestFilenameMsg = (CCSDef::TMSG_FILENAME*)pMsgHeader;
	TRACE("File Name: %s\n", pRequestFilenameMsg->szFileName);

	// ���ļ���·������ΪC�̸�Ŀ¼��
	strcpy(g_szFileName, "c:\\");
	strcat(g_szFileName, pRequestFilenameMsg->szFileName);

	// ������ͬ�ļ������ļ��Ƿ��Ѿ�����,������ڱ����˳�
	FILE* pFile;
	if (NULL != (pFile = fopen(g_szFileName, "r")))
	{
		// �ļ��Ѿ�����,Ҫ����������һ���ļ�
		TRACE("The file already exist!\n");
		CCSDef::TMSG_ERROR_MSG tMsgErrorMsg(MSG_FILEALREADYEXIT_ERROR);
		::send(g_sClient, (char*)(&tMsgErrorMsg), sizeof(CCSDef::TMSG_ERROR_MSG), 0);

		fclose(pFile);
		return false;
	}

	// ���仺������ʼ�����ļ�,�������ɹ��͸�server�˷��Ϳ�ʼ�����ļ���Ҫ��
	g_pBuff = new char[g_lLength + 1];
	if (NULL != g_pBuff)
	{
		memset(g_pBuff, '\0', g_lLength + 1);
		TRACE("Now ready to get the file %s!\n", pRequestFilenameMsg->szFileName);
		CCSDef::TMSG_CLIENT_READY tMsgClientReady;

		if (SOCKET_ERROR == ::send(g_sClient, (char*)(&tMsgClientReady), sizeof(CCSDef::TMSG_CLIENT_READY), 0))
		{
			AfxMessageBox("Send Error!\n");
			return false;
		}
	}
	else
	{
		AfxMessageBox("Alloc memory for file error!\n");
		return false;
	}

	return true;
}

// д���ļ�
bool CClient::WriteToFile(CCSDef::TMSG_HEADER *pMsgHeader)
{
	assert(NULL != pMsgHeader);

	CCSDef::TMSG_FILE* pMsgFile = (CCSDef::TMSG_FILE*)pMsgHeader;

	int nStart = pMsgFile->tFile.nStart;
	int nSize = pMsgFile->tFile.nSize;
	memcpy(g_pBuff + nStart, pMsgFile->tFile.szBuff, nSize);
	if (0 == nStart)
	{
		TRACE("Saving file into buffer...\n");
	}

	memcpy(g_pBuff + nStart, pMsgFile->tFile.szBuff, nSize);
	//AfxMessageBox("start = %d, size = %d\n", nStart, nSize);

	// ����Ѿ����浽��������Ͼ�д���ļ�
	if (nStart + nSize >= g_lLength)
	{
		TRACE("Writing to disk....\n");
		// д���ļ�
		FILE* pFile;
		pFile = fopen(g_szFileName, "w+b");
		fwrite(g_pBuff, sizeof(char), g_lLength, pFile);

		delete [] g_pBuff;
		g_pBuff = NULL;
		fclose(pFile);

		// �����ļ��ɹ�������Ϣ��server�˳�server
		CCSDef::TMSG_SENDFILESUCCESS tMsgSendFileSuccess;
		while (SOCKET_ERROR == ::send(g_sClient, (char*)(&tMsgSendFileSuccess), sizeof(CCSDef::TMSG_SENDFILESUCCESS), 0))
		{
		}

		TRACE("Save the file %s success!\n", g_szFileName);

		return true;
	}
	else
	{
		return false;
	}
}

