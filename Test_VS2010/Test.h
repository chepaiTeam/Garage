// Test.h : main header file for the TEST application
//

#if !defined(AFX_TEST_H__21057015_7DBB_409B_8E56_F9ACADA1E43D__INCLUDED_)
#define AFX_TEST_H__21057015_7DBB_409B_8E56_F9ACADA1E43D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define SYSSETPATHNAME   _T(".\\SysSet.ini")

/////////////////////////////////////////////////////////////////////////////
// CTestApp:
// See Test.cpp for the implementation of this class
//
class CTestApp : public CWinApp
{
public:
	CTestApp();
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//读取字符串
	static CString GetIniString(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  LPCTSTR  lpDefault);
	//写入字符串
	static UINT WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	//读取整形数据
	static UINT GetIniInt(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  INT nDefault);

	//{{AFX_MSG(CTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__21057015_7DBB_409B_8E56_F9ACADA1E43D__INCLUDED_)
