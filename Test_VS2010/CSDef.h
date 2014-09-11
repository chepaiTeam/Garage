#pragma once

#define MAX_PACKET_SIZE   10240    // 数据包的最大长度,单位是sizeof(char)
#define MAXFILEDIRLENGTH 256     // 存放文件路径的最大长度
#define PORT     4096    // 端口号
#define SERVER_IP    "127.0.0.1" // server端的IP地址

// 各种消息的宏定义
#define INVALID_MSG      -1   // 无效的消息标识
#define MSG_FILENAME     1   // 文件的名称
#define MSG_FILELENGTH     2   // 传送文件的长度
#define MSG_CLIENT_READY    3   // 客户端准备接收文件
#define MSG_FILE      4   // 传送文件
#define MSG_SENDFILESUCCESS    5   // 传送文件成功
#define MSG_OPENFILE_ERROR    10   // 打开文件失败,可能是文件路径错误找不到文件等原因
#define MSG_FILEALREADYEXIT_ERROR 11   // 要保存的文件已经存在了

class CCSDef
{
public:
#pragma pack(1)      // 使结构体的数据按照1字节来对齐,省空间

	// 消息头
	struct TMSG_HEADER
	{
		char    cMsgID;    // 消息标识

		TMSG_HEADER(char MsgID = INVALID_MSG)
			: cMsgID(MsgID)
		{
		}
	};

	// 请求传送的文件名
	// 客户端传给服务器端的是全路径名称
	// 服务器传回给客户端的是文件名
	struct TMSG_FILENAME : public TMSG_HEADER
	{
		char szFileName[256];   // 保存文件名的字符数组

		TMSG_FILENAME()
			: TMSG_HEADER(MSG_FILENAME)
		{
		}
	};

	// 传送文件长度
	struct TMSG_FILELENGTH : public TMSG_HEADER
	{
		long lLength;

		TMSG_FILELENGTH(long length)
			: TMSG_HEADER(MSG_FILELENGTH), lLength(length) 
		{

		}
	};

	// Client端已经准备好了,要求Server端开始传送文件
	struct TMSG_CLIENT_READY : public TMSG_HEADER
	{
		TMSG_CLIENT_READY()
			: TMSG_HEADER(MSG_CLIENT_READY)
		{
		}
	};

	// 传送文件
	struct TMSG_FILE : public TMSG_HEADER
	{
		union     // 采用union保证了数据包的大小不大于MAX_PACKET_SIZE * sizeof(char)
		{
			char szBuff[MAX_PACKET_SIZE];
			struct
			{
				int nStart;
				int nSize;
				char szBuff[MAX_PACKET_SIZE - 2 * sizeof(int)];
			}tFile;
		};

		TMSG_FILE()
			: TMSG_HEADER(MSG_FILE)
		{

		}
	};

	// 传送文件成功
	struct TMSG_SENDFILESUCCESS : public TMSG_HEADER
	{
		TMSG_SENDFILESUCCESS()
			: TMSG_HEADER(MSG_SENDFILESUCCESS)
		{
		}
	};

	// 传送出错信息,包括:
	// MSG_OPENFILE_ERROR:打开文件失败
	// MSG_FILEALREADYEXIT_ERROR:要保存的文件已经存在了
	struct TMSG_ERROR_MSG : public TMSG_HEADER
	{
		TMSG_ERROR_MSG(char cErrorMsg)
			: TMSG_HEADER(cErrorMsg)
		{
		}
	};

#pragma pack()
};