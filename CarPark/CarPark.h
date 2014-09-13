
// CarPark.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define	NETTYPE_CLIENT 1
#define	NETTYPE_SERVER 2

#define SYSSETPATHNAME   _T(".\\SysSet.ini")

// CCarParkApp:
// 有关此类的实现，请参阅 CarPark.cpp
//

class CCarParkApp : public CWinApp
{
public:
	CCarParkApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	//读取字符串
	static CString GetIniString(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  LPCTSTR  lpDefault);
	//写入字符串
	static UINT WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	//读取整形数据
	static UINT GetIniInt(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  INT nDefault);
};

extern CCarParkApp theApp;