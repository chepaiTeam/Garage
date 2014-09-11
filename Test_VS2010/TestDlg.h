// TestDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__C89B8F3E_20E0_4E86_9848_B527C645A545__INCLUDED_)
#define AFX_TESTDLG_H__C89B8F3E_20E0_4E86_9848_B527C645A545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	int nID;					//ID
	int nEffective;				//是否有效  0：无效   1：有效
	int nBaseNum;				//检测基数
	int nOffset;				//检测基数偏移量
	int nRectX;					//
	int nRectY;					//
	int nRectW;					//
	int nRectH;					//
	int nIsCar;					//是否有车  0：没有   1：有
} CAM_INFO, *pCAM_INFO;

typedef struct
{
	int nMin;		//负代表无限小
	int nMax;		//负代表无限大
	CString sRgb;	//颜色值
} RGB_INFO, *pRGB_INFO;

typedef struct
{
	CString sAddressCode;				//地址码
	unsigned int uLocation;				//区位
	CString sDirection;					//方向
	unsigned int uParkingLotNum;		//车位数量
	CArray<RGB_INFO*, RGB_INFO*&> TextRgbs;			//文字颜色范围
	CArray<RGB_INFO*, RGB_INFO*&> TagRgbs;			//方向颜色范围
} LED_INFO, *pLED_INFO;


/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
#include "VideoView.h"
#include "AviFileObj.h"
#include "MediaFile.h"
#include "afxwin.h"
#define TIMER_MAIN    WM_USER+100


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

// Implementation
protected:
	CString GetBaseDir(const CString &path);
	void ShowHideScreen();
	void InitRect();
	void OnButton2();
	void OnButton3();
	void SetParame(int nChannel);

	HICON m_hIcon;

	CSerialPort* m_pPort1;							//串口1
	CSerialPort* m_pPort2;							//串口2

	CString m_sNullCarStr;

	bool m_bDataRun;
	HANDLE hThread;
	DWORD ThreadID;

	HANDLE hLedThread;
	DWORD LedThreadID;

	int m_nFrameRate;								//帧率
	int m_framerate[MAX_VIDEO_CHANNEL];				//各通道帧率

	CString strDir;									//SetDefaultDisplayPicture 默认显示图片
	int m_nindex;									//StartVideoOutput 视频输入记数
	int m_nCount;									//GPIO相关

	RECT m_Onerect;									//显示区域
	int m_Current;									//当前选定显示视频框
	BOOL m_bOne;									//子视频框是否放大
	int m_nSubChannel;								//与录像中的帧率有关，具体未知
	int m_nViewMode;								//子视频框布局 2x2 3x3 4x4 6x6
	BOOL m_bCapture;								//是否开始录像

	//CVideoView m_SrcVideoView, m_ConVideoView;

	CVideoView *m_pVideoView[MAX_VIDEO_CHANNEL];	//子视频框窗体绑定
	CAviFileObj *m_pAviFile[MAX_VIDEO_CHANNEL];		//各通道视频文件，具体未知
	BOOL m_bCanWriteFile[MAX_VIDEO_CHANNEL];		//可写视频文件标志
	BOOL m_bRecord[MAX_VIDEO_CHANNEL];				//录像启停标志
	CMediaFile *m_pMediaFile[MAX_VIDEO_CHANNEL];	//录像文件
	unsigned long lBitRateValue;					//比特率，视频流大小。

	static void DataThread(LPVOID pParam);

	static void CVData(LPVOID pParam);

	static void LedThread(LPVOID pParam);

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnButtonInit();
	afx_msg void OnButton2x2();
	afx_msg void OnButton3x3();
	afx_msg void OnButton4x4();
	afx_msg void OnButton6x6();
	//}}AFX_MSG
	afx_msg LRESULT OnMsgViewDbClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgViewClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	LED_INFO m_LedInfo;
	CAM_INFO *m_pCamInfos;
	CArray<BYTE *, BYTE *&> m_aBuffers;
	CStatic m_stcXD[4];
	CEdit m_edtXH[4];
	int m_nIDs[4];
	CComboBox m_cboXD;
	CButton m_btnBC;
	afx_msg void OnBnClickedButton36();
	afx_msg void OnBnClickedButton37();
	CButton m_btnYL;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__C89B8F3E_20E0_4E86_9848_B527C645A545__INCLUDED_)
