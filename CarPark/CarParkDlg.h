
// CarParkDlg.h : ͷ�ļ�
//

#pragma once


#include "Server.h"
#include "Client.h"

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

	CStatic m_stcXD[4];								//ѡ��ͨ���ź���
	CEdit m_edtXH[4];								//����ѡ��ͨ���ź�����ֵ
	int m_nIDs[4];									//��ǰѡ��ͨ����4������ͷID
	CComboBox m_cboXD;								
	CButton m_btnBC;

	CButton m_btnYL;
	CStatic m_stcNet;
	CButton m_rdoClient;
	CButton m_rdoServer;

	//������
	CServer *m_pServer;
	//�ͻ���
	CClient *m_pClient;
	//�豸�Ƿ��ʼ��
	BOOL m_bDeviceInit;

// Implementation
protected:
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

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonOpenView();

	DECLARE_MESSAGE_MAP()
};
