#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "customeditctrl.h"


// CServerSetDlg �Ի���

class CServerSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerSetDlg)

public:
	CServerSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServerSetDlg();

// �Ի�������
	enum { IDD = IDD_SERVERSET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listLedInfo;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edtAddress;
	CComboBox m_cboDirection;
	CCustomEditCtrl m_edtCarNum;
	CComboBox m_cboComName;

	CListBox m_boxCarGroup;
	CEdit m_edtCarGroup;

	CListBox m_boxTextFormat;
	CCustomEditCtrl m_edtTextFormatMin;
	CCustomEditCtrl m_edtTextFormatMax;
	CComboBox m_cboTextFormatColor;

	CListBox m_boxTagFormat;
	CCustomEditCtrl m_edtTagFormatMin;
	CCustomEditCtrl m_edtTagFormatMax;
	CComboBox m_cboTagFormatColor;

	//��λ��
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	//���ָ�ʽ
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();

	//��ǩ��ʽ
	afx_msg void OnLbnSelchangeList4();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButton10();

	//����list����
	BOOL m_bListAction; 

	//�����Ƿ�ı�
	BOOL m_bIsChange;
	CEdit m_edtLedName;
};
