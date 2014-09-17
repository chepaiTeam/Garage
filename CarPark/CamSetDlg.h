#pragma once
#include "afxwin.h"
#include "CustomEditCtrl.h"

// CCamSetDlg �Ի���

class CCamSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamSetDlg)

public:
	CCamSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCamSetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CAMSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
protected:
	CEdit m_edtCamID;
	CEdit m_edtGroupID;
	CCustomEditCtrl m_edtBase;
	CCustomEditCtrl m_edtOffset;
	CCustomEditCtrl m_edtX;
	CCustomEditCtrl m_edtY;
	CCustomEditCtrl m_edtW;
	CCustomEditCtrl m_edtH;
	CButton m_btnStart;

	CAM_INFO *m_pCamInfo;
public:
	void SetCamInfo(CAM_INFO *pCamInfo);
	CEdit m_edtCarParkName;
};
