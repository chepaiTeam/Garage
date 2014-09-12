#pragma once

#define MAX_PACKET_SIZE			10240    // 数据包的最大长度,单位是sizeof(char)
#define MAXFILEDIRLENGTH		256     // 存放文件路径的最大长度
#define SERVER_PORT_CAST		4096    // 服务器端口号
#define SERVER_PORT_TCP			4097    // 服务器端口号
#define CAST_PORT				4098    // 广播端口号
#define CLIENT_PORT_TCP			4099    // 客户端端口号


// 各种消息的宏定义
#define INVALID_MSG					-1		// 无效的消息标识
#define MSG_DEVICEINFO				 1		// PC设备信息
#define MSG_LEDINFO					 2		// LED设备信息
#define MSG_OPENFILE_ERROR			 10		// 错误信息
#define MSG_FILEALREADYEXIT_ERROR	 11		// 错误信息

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

	// 由客户端传送给服务器的当前设备信息
	struct TMSG_DEVICEINFO : public TMSG_HEADER
	{
		char szDeviceName[256];		// 设备名
		int nRedLightNum;			// 红灯数
		int nGreenLightNum;			// 绿灯数
		int nNoneLightNum;			// 未使用灯数

		TMSG_DEVICEINFO()
			: TMSG_HEADER(MSG_DEVICEINFO)
		{
		}
	};

	// 由服务器传送给客户端的当前LED信息
	struct TMSG_LEDINFO : public TMSG_HEADER
	{
		char szBuff[MAX_PACKET_SIZE];

		TMSG_LEDINFO()
			: TMSG_HEADER(MSG_LEDINFO)
		{
		}
	};


	// 传送出错信息,包括:
	struct TMSG_ERROR_MSG : public TMSG_HEADER
	{
		TMSG_ERROR_MSG(char cErrorMsg)
			: TMSG_HEADER(cErrorMsg)
		{
		}
	};

	//// 传送文件
	//struct TMSG_FILE : public TMSG_HEADER
	//{
	//	union     // 采用union保证了数据包的大小不大于MAX_PACKET_SIZE * sizeof(char)
	//	{
	//		char szBuff[MAX_PACKET_SIZE];
	//		struct
	//		{
	//			int nStart;
	//			int nSize;
	//			char szBuff[MAX_PACKET_SIZE - 2 * sizeof(int)];
	//		}tFile;
	//	};

	//	TMSG_FILE()
	//		: TMSG_HEADER(MSG_FILE)
	//	{

	//	}
	//};

#pragma pack()
};