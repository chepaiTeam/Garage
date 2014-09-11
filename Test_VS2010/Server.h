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
	// 关闭socket库
	bool CloseSocket();
	// 解析消息进行相应的处理
	bool ProcessMsg(SOCKET sClient);
	// 监听Client的消息
	void ListenToClient();
	// 打开文件
	bool OpenFile(CCSDef::TMSG_HEADER* pMsgHeader, SOCKET sClient);
	// 传送文件
	bool SendFile(SOCKET sClient);
	// 读取文件进入缓冲区
	bool ReadFile(SOCKET sClient);
};

