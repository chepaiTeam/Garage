
// CarPark.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CarPark.h"
#include "CarParkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarParkApp

BEGIN_MESSAGE_MAP(CCarParkApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarParkApp ����

CCarParkApp::CCarParkApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCarParkApp ����

CCarParkApp theApp;


// CCarParkApp ��ʼ��

BOOL CCarParkApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	if(g_DB.Open(
		"Provider=Microsoft.Jet.OLEDB.4.0;\
		Data Source=Data\\sdk3V.mdb;\
		Persist Security Info=False;\
		Jet OLEDB:Database Password=630022474"
		))
	{
		AfxMessageBox("�����ݿ�ʧ�ܣ�");
		return FALSE;
	}

	g_sHostName = GetIniString(_T("Device"), _T("HostName"), _T("S01"));

	g_nNetType = GetIniInt(_T("NetData"), _T("NetType"), NETTYPE_CLIENT);

	// ��ʼ��socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	if (::WSAStartup(socketVersion, &wsaData) != 0)
	{
		AfxMessageBox("Init socket dll error\n");
		return FALSE;
	}

	GdiplusStartupInput input;  
	GdiplusStartup(&m_gdiplusToken, &input, NULL); 

	g_bAppRun = true;

	CCarParkDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


int CCarParkApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CString str;
	str.Format(_T("%d"), g_nNetType);
	WriteIniString(_T("NetData"), _T("NetType"), str);

	// �ͷ�winsock��
	::WSACleanup();

	GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}

CString CCarParkApp::GetIniString(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  LPCTSTR  lpDefault)
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

UINT CCarParkApp::WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
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

UINT CCarParkApp::GetIniInt(LPCTSTR lpAppName, LPCTSTR   lpKeyName,  INT   nDefault)
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