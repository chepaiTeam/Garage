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
	int nEffective;				//�Ƿ���Ч  0����Ч   1����Ч
	int nBaseNum;				//������
	int nOffset;				//������ƫ����
	int nRectX;					//
	int nRectY;					//
	int nRectW;					//
	int nRectH;					//
	int nIsCar;					//�Ƿ��г�  0��û��   1����
} CAM_INFO, *pCAM_INFO;

typedef struct
{
	int nMin;		//����������С
	int nMax;		//���������޴�
	CString sRgb;	//��ɫֵ
} RGB_INFO, *pRGB_INFO;

typedef struct
{
	CString sAddressCode;				//��ַ��
	unsigned int uLocation;				//��λ
	CString sDirection;					//����
	unsigned int uParkingLotNum;		//��λ����
	CArray<RGB_INFO*, RGB_INFO*&> TextRgbs;			//������ɫ��Χ
	CArray<RGB_INFO*, RGB_INFO*&> TagRgbs;			//������ɫ��Χ
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

	CSerialPort* m_pPort1;							//����1
	CSerialPort* m_pPort2;							//����2

	CString m_sNullCarStr;

	bool m_bDataRun;
	HANDLE hThread;
	DWORD ThreadID;

	HANDLE hLedThread;
	DWORD LedThreadID;

	int m_nFrameRate;								//֡��
	int m_framerate[MAX_VIDEO_CHANNEL];				//��ͨ��֡��

	CString strDir;									//SetDefaultDisplayPicture Ĭ����ʾͼƬ
	int m_nindex;									//StartVideoOutput ��Ƶ�������
	int m_nCount;									//GPIO���

	RECT m_Onerect;									//��ʾ����
	int m_Current;									//��ǰѡ����ʾ��Ƶ��
	BOOL m_bOne;									//����Ƶ���Ƿ�Ŵ�
	int m_nSubChannel;								//��¼���е�֡���йأ�����δ֪
	int m_nViewMode;								//����Ƶ�򲼾� 2x2 3x3 4x4 6x6
	BOOL m_bCapture;								//�Ƿ�ʼ¼��

	//CVideoView m_SrcVideoView, m_ConVideoView;

	CVideoView *m_pVideoView[MAX_VIDEO_CHANNEL];	//����Ƶ�����
	CAviFileObj *m_pAviFile[MAX_VIDEO_CHANNEL];		//��ͨ����Ƶ�ļ�������δ֪
	BOOL m_bCanWriteFile[MAX_VIDEO_CHANNEL];		//��д��Ƶ�ļ���־
	BOOL m_bRecord[MAX_VIDEO_CHANNEL];				//¼����ͣ��־
	CMediaFile *m_pMediaFile[MAX_VIDEO_CHANNEL];	//¼���ļ�
	unsigned long lBitRateValue;					//�����ʣ���Ƶ����С��

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
