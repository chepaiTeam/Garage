
// CarParkDlg.h : 头文件
//

#pragma once


#include "Server.h"
#include "Client.h"

// CCarParkDlg 对话框
class CCarParkDlg : public CDialogEx
{
// 构造
public:
	CCarParkDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CCarParkDlg();

// 对话框数据
	enum { IDD = IDD_CARPARK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
//Data
protected:
	CString strDir;									//SetDefaultDisplayPicture 默认显示图片

	RECT m_Onerect;									//显示区域
	int m_Current;									//当前选定显示视频框
	BOOL m_bOne;									//子视频框是否放大
	int m_nViewMode;								//子视频框布局 2x2 3x3 4x4 6x6

	//CVideoView m_SrcVideoView, m_ConVideoView;
	CVideoView *m_pVideoView[MAX_VIDEO_CHANNEL];	//子视频框窗体绑定

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
	//设备是否初始化
	BOOL m_bDeviceInit;

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

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnButton2x2();
	afx_msg void OnButton3x3();
	afx_msg void OnButton4x4();
	afx_msg void OnButton6x6();

	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnMsgViewDbClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgViewClick(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonOpenView();

	DECLARE_MESSAGE_MAP()
};
