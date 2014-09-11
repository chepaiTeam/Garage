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
	// 关闭socket库
	bool CloseSocket();
	// 把用户输入的文件路径传送到server端
	bool SendFileNameToServer();
	// 与server端连接
	bool ConectToServer();
	// 打开文件失败
	bool OpenFileError(CCSDef::TMSG_HEADER *pMsgHeader);
	// 分配空间以便写入文件
	bool AllocateMemoryForFile(CCSDef::TMSG_HEADER *pMsgHeader);
	// 写入文件
	bool WriteToFile(CCSDef::TMSG_HEADER *pMsgHeader);
	// 处理server端传送过来的消息
	bool ProcessMsg();
};

