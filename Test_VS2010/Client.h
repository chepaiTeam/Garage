#pragma once
#include "CSDef.h"

class CClient
{
public:
	CClient(void);
	~CClient(void);
protected:
	long g_lLength;
	char* g_pBuff;
	char g_szFileName[MAXFILEDIRLENGTH];
	char g_szBuff[MAX_PACKET_SIZE + 1];
	SOCKET g_sClient;
public:
	// �ر�socket��
	bool CloseSocket();
	// ���û�������ļ�·�����͵�server��
	bool SendFileNameToServer();
	// ��server������
	bool ConectToServer();
	// ���ļ�ʧ��
	bool OpenFileError(CCSDef::TMSG_HEADER *pMsgHeader);
	// ����ռ��Ա�д���ļ�
	bool AllocateMemoryForFile(CCSDef::TMSG_HEADER *pMsgHeader);
	// д���ļ�
	bool WriteToFile(CCSDef::TMSG_HEADER *pMsgHeader);
	// ����server�˴��͹�������Ϣ
	bool ProcessMsg();
};

