/*server.cpp*/

#include "CSDef.h"


class CServer
{
public:
	CServer();
	~CServer();
protected:
	char g_szNewFileName[MAXFILEDIRLENGTH];
	char g_szBuff[MAX_PACKET_SIZE + 1];
	long g_lLength;
	char* g_pBuff;

public:
	// �ر�socket��
	bool CloseSocket();
	// ������Ϣ������Ӧ�Ĵ���
	bool ProcessMsg(SOCKET sClient);
	// ����Client����Ϣ
	void ListenToClient();
	// ���ļ�
	bool OpenFile(CCSDef::TMSG_HEADER* pMsgHeader, SOCKET sClient);
	// �����ļ�
	bool SendFile(SOCKET sClient);
	// ��ȡ�ļ����뻺����
	bool ReadFile(SOCKET sClient);
};

