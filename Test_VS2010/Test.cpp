// Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestApp

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
	//{{AFX_MSG_MAP(CTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestApp construction

CTestApp::CTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestApp object

CTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTestApp initialization

BOOL CTestApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//short u,v;
	//BYTE r,g,b;
	//BYTE bU,bV,bY;
	//COLORREF color;
	//color = RGB(0,0,255);
	//r = GetRValue(color);
	//g = GetGValue(color);
	//b = GetBValue(color);
	//bY = (BYTE)(0.299*r+0.587*g+0.114*b);
	//u = (short)(-0.147*r-0.289*g+0.436*b);
	//v = (short)(0.615*r-0.515*g-0.100*b);
	//bU = min(255,u+128);
	//bV = min(255,v+128);

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	AfxOleInit();
	if(g_DB.Open(
		"Provider=Microsoft.Jet.OLEDB.4.0;\
		Data Source=Data\\sdk3V.mdb;\
		Persist Security Info=False;\
		Jet OLEDB:Database Password=630022474"
		))
	{
		AfxMessageBox("打开数据库失败！");
		return FALSE;
	}

	// 初始化socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	if (::WSAStartup(socketVersion, &wsaData) != 0)
	{
		AfxMessageBox("Init socket dll error\n");
		return FALSE;
	}

	CTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CTestApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	// 释放winsock库
	::WSACleanup();
	return CWinApp::ExitInstance();
}


CString CTestApp::GetIniString(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  LPCTSTR  lpDefault)
{
	try
	{
		CString str;
		DWORD DW=GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,str.GetBuffer(MAX_PATH),MAX_PATH,SYSSETPATHNAME);
		return str;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

UINT CTestApp::WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	try
	{
		return WritePrivateProfileString(lpAppName, lpKeyName, lpString,  SYSSETPATHNAME);  
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

UINT CTestApp::GetIniInt(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  INT   nDefault)
{
	try
	{
		return GetPrivateProfileInt(lpAppName,lpKeyName,nDefault, SYSSETPATHNAME);  
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
}