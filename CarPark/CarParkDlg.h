
// CarParkDlg.h : ͷ�ļ�
//

#pragma once


#include "Server.h"
#include "Client.h"
#include "LightBtn.h"
#include "StaticEx.h"
#include "afxwin.h"

// CCarParkDlg �Ի���
class CCarParkDlg : public CDialogEx
{
// ����
public:
	CCarParkDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CCarParkDlg();

// �Ի�������
	enum { IDD = IDD_CARPARK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
//Data
protected:
	CString strDir;									//SetDefaultDisplayPicture Ĭ����ʾͼƬ

	RECT m_Onerect;									//��ʾ����
	int m_Current;									//��ǰѡ����ʾ��Ƶ��
	BOOL m_bOne;									//����Ƶ���Ƿ�Ŵ�
	int m_nViewMode;								//����Ƶ�򲼾� 2x2 3x3 4x4 6x6

	//CVideoView m_SrcVideoView, m_ConVideoView;
	CVideoView *m_pVideoView[MAX_VIDEO_CHANNEL];	//����Ƶ�����

	CComboBox m_cboXD;					
	CButton m_btnBC;

	CButton m_btnYL;
	CButton m_rdoClient;
	CButton m_rdoServer;

	//������
	CServer *m_pServer;
	//�ͻ���
	CClient *m_pClient;

	Image *m_pImgLightRed;		//���
	Image *m_pImgLightGreen;	//�̵�
	Image *m_pImgLightRG;		//���̵�
	Image *m_pImgLightNone;		//Ϩ��
	
	CLightBtn m_LBtn[64];
	CStaticEx m_stcCamInfo[64];

	CWnd *m_wSelStc;

	CComboBox m_cboRunModel;
// Implementation
protected:
	int LoadImageFromResource(IN Image * &pImg,
		IN UINT nResID, 
		IN LPCSTR lpTyp);

	CString GetBaseDir(const CString &path);
	void ShowHideScreen();
	void InitRect();

	/// <summary> 
	/// �������ݿ�������ͷ����������Ϣ
	/// </summary> 
	void LoadCamData();


	/// <summary> 
	/// �豸��ʼ��
	/// </summary> 
	void DeviceInit();

	/// <summary> 
	/// �ɼ�������ʼ��
	/// </summary> 
	void ParameInit();

	/// <summary> 
	/// �����豸
	/// </summary> 
	void StartDevice();

	static void CamThread(LPVOID pParam);
	static void CVData(LPVOID pParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	afx_msg void OnBnClickedButtonOpenView();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButton(UINT uID);
	afx_msg void OnStnClickedStatic(UINT uID);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboModle();
	afx_msg BOOL SetTipText(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	void SetSelStc(CWnd *wSelStc);
	void InitLight(int nLightType);
	void CtrlLight(int nID, int nLightType);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	CButton m_btnServerSet;
};
