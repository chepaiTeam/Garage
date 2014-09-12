// stdafx.cpp : source file that includes just the standard includes
//	Test.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CDataBase g_DB;
bool g_bAppRun;
CString g_sDeviceID;							//设备编号
int g_nNetType;									//该控制程序的网络类型 NETTYPE_CLIENT:客户端  NETTYPE_SERVER：服务器
CArray<CAM_INFO *, CAM_INFO *&> g_pCamInfos;							//相机信息结构体

CSerialPort* g_pPort1;							//串口1
CSerialPort* g_pPort2;							//串口2