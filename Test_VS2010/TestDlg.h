// TestDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__C89B8F3E_20E0_4E86_9848_B527C645A545__INCLUDED_)
#define AFX_TESTDLG_H__C89B8F3E_20E0_4E86_9848_B527C645A545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
#include "VideoView.h"
#include "afxwin.h"
#include "Server.h"
#include "Client.h"

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CTestDlg();

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_DIALOG1 };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

//Data
protected:
	CString strDir;									//SetDefaultDisplayPicture 默认显示图片

	RECT m_Onerect;									//显示区域
	int m_Current;									//当前选定显示视频框
	BOOL m_bOne;									//子视频框是否放大
	int m_nViewMode;								//子视频框布局 2x2 3x3 4x4 6x6

	//CVideoView m_SrcVideoView, m_ConVideoView;
	CVideoView *m_pVideoView[MAX_VIDEO_CHANNEL];	//子视频框窗体绑定

	CArray<BYTE *, BYTE *&> m_aBuffers;				//通道缓存数据
	CStatic m_stcXD[4];								//选中通道信号量
	CEdit m_edtXH[4];								//设置选中通道信号量阀值
	int m_nIDs[4];									//当前选中通道的4个摄像头ID
	CComboBox m_cboXD;								
	CButton m_btnBC;

	CButton m_btnYL;
	CStatic m_stcNet;
	CButton m_rdoClient;
	CButton m_rdoServer;

	//服务器
	CServer *m_pServer;
	//客户端
	CClient *m_pClient;

// Implementation
protected:
	CString GetBaseDir(const CString &path);
	void ShowHideScreen();
	void InitRect();

	/// <summary> 
	/// 加载数据库中摄像头数据数据信息
	/// </summary> 
	void LoadCamData();


	/// <summary> 
	/// 设备初始化
	/// </summary> 
	void DeviceInit();

	/// <summary> 
	/// 采集参数初始化
	/// </summary> 
	void ParameInit();

	/// <summary> 
	/// 启动设备
	/// </summary> 
	void StartDevice();

	static void CamThread(LPVOID pParam);
	static void CVData(LPVOID pParam);

protected:
	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButton2x2();
	afx_msg void OnButton3x3();
	afx_msg void OnButton4x4();
	afx_msg void OnButton6x6();
	//}}AFX_MSG
	afx_msg LRESULT OnMsgViewDbClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgViewClick(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButton36();
	afx_msg void OnBnClickedButton37();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__C89B8F3E_20E0_4E86_9848_B527C645A545__INCLUDED_)
