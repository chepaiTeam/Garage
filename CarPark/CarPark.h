
// CarPark.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define	NETTYPE_CLIENT 1
#define	NETTYPE_SERVER 2

#define SYSSETPATHNAME   _T(".\\SysSet.ini")

// CCarParkApp:
// �йش����ʵ�֣������ CarPark.cpp
//

class CCarParkApp : public CWinApp
{
public:
	CCarParkApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	//��ȡ�ַ���
	static CString GetIniString(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  LPCTSTR  lpDefault);
	//д���ַ���
	static UINT WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	//��ȡ��������
	static UINT GetIniInt(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  INT nDefault);
};

extern CCarParkApp theApp;