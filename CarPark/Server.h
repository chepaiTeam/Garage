#pragma once
#include "CSDef.h"

typedef struct
{
	int nMin;		//负代表无限小
	int nMax;		//负代表无限大
	CString sRgb;	//颜色值
} RGB_INFO, *pRGB_INFO;

typedef struct
{
	CString sAddress;				//地址码+区位
	CString sDirection;					//方向
	unsigned int uParkingLotNum;		//车位数量
	CString sTextRgb;					//文字颜色字符串
	CString sTagRgb;					//方向颜色字符串
	CArray<RGB_INFO*, RGB_INFO*&> TextRgbs;			//文字颜色范围
	CArray<RGB_INFO*, RGB_INFO*&> TagRgbs;			//方向颜色范围
	int nComNum;					//串口位置
} LED_INFO, *pLED_INFO;


typedef struct
{
	CString sGroupID;
	CString sLedAddress;
	int nEffective;
	int nRedLightNum;
	int nGreenLightNum;
	int nRGLightNum;
	int nNoneLightNum;
} GROUP_INFO, *pGROUP_INFO;

typedef struct
{
	CString sIP;
	CArray<GROUP_INFO*> aGroupInfos;
} DEVICE_INFO, *pDEVICE_INFO;

class CServer
{
public:
	CServer(void);
	~CServer(void);
protected:

public:
	/// <summary> 
	/// 加载数据库中LED设备数据信息
	/// </summary> 
	void LoadData();

	// 关闭socket库
	bool CloseSocket();
	// 与server端连接
	bool InitServer();

	/// <summary> 
	/// 发送消息线程
	/// </summary> 
	static void BroadcastThread(LPVOID pParam);


	/// <summary> 
	/// 启动服务器线程
	/// </summary> 
	static void StartServerThread(LPVOID pParam);

	/// <summary> 
	/// 接收消息线程
	/// </summary> 
	static void STCPRecvThread(LPVOID pParam);
};

