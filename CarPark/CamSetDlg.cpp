// CamSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarPark.h"
#include "CamSetDlg.h"
#include "afxdialogex.h"


// CCamSetDlg 对话框

IMPLEMENT_DYNAMIC(CCamSetDlg, CDialogEx)

CCamSetDlg::CCamSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCamSetDlg::IDD, pParent)
{
	m_pCamInfo = NULL;
}

CCamSetDlg::~CCamSetDlg()
{
}

void CCamSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtCamID);
	DDX_Control(pDX, IDC_EDIT7, m_edtGroupID);
	DDX_Control(pDX, IDC_EDIT2, m_edtBase);
	DDX_Control(pDX, IDC_EDIT6, m_edtOffset);
	DDX_Control(pDX, IDC_EDIT4, m_edtX);
	DDX_Control(pDX, IDC_EDIT8, m_edtY);
	DDX_Control(pDX, IDC_EDIT9, m_edtW);
	DDX_Control(pDX, IDC_EDIT10, m_edtH);
	DDX_Control(pDX, IDC_CHECK1, m_btnStart);
	DDX_Control(pDX, IDC_EDIT13, m_edtCarParkName);
}


BEGIN_MESSAGE_MAP(CCamSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCamSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCamSetDlg 消息处理程序

BOOL CCamSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edtBase.SetNumericEditBox(0, 10000, 1, 0, FALSE);
	m_edtOffset.SetNumericEditBox(0, 10000, 1, 0, FALSE);
	m_edtX.SetNumericEditBox(0, 2000, 1, 0, FALSE);
	m_edtY.SetNumericEditBox(0, 2000, 1, 0, FALSE);
	m_edtW.SetNumericEditBox(0, 2000, 1, 0, FALSE);
	m_edtH.SetNumericEditBox(0, 2000, 1, 0, FALSE);

	if (m_pCamInfo)
	{
		CString str;
		str.Format(_T("%02d"), m_pCamInfo->nID);
		m_edtCamID.SetWindowText(str);
		m_edtCarParkName.SetWindowText(m_pCamInfo->sCarParkName);
		m_edtGroupID.SetWindowText(m_pCamInfo->sGroupID);
		str.Format(_T("%d"), m_pCamInfo->nBaseNum);
		m_edtBase.SetWindowText(str);
		str.Format(_T("%d"), m_pCamInfo->nOffset);
		m_edtOffset.SetWindowText(str);
		str.Format(_T("%d"), m_pCamInfo->nRectX);
		m_edtX.SetWindowText(str);
		str.Format(_T("%d"), m_pCamInfo->nRectY);
		m_edtY.SetWindowText(str);
		str.Format(_T("%d"), m_pCamInfo->nRectW);
		m_edtW.SetWindowText(str);
		str.Format(_T("%d"), m_pCamInfo->nRectH);
		m_edtH.SetWindowText(str);
		m_btnStart.SetCheck(m_pCamInfo->nEffective);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCamSetDlg::SetCamInfo(CAM_INFO *pCamInfo)
{
	m_pCamInfo = pCamInfo;
}

void CCamSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTemp;
	m_edtCarParkName.GetWindowText(m_pCamInfo->sCarParkName);
	m_edtGroupID.GetWindowText(m_pCamInfo->sGroupID);
	m_edtBase.GetWindowText(sTemp);
	m_pCamInfo->nBaseNum = _ttoi(sTemp);
	m_edtOffset.GetWindowText(sTemp);
	m_pCamInfo->nOffset = _ttoi(sTemp);
	m_edtX.GetWindowText(sTemp);
	m_pCamInfo->nRectX = _ttoi(sTemp);
	m_edtY.GetWindowText(sTemp);
	m_pCamInfo->nRectY = _ttoi(sTemp);
	m_edtW.GetWindowText(sTemp);
	m_pCamInfo->nRectW = _ttoi(sTemp);
	m_edtH.GetWindowText(sTemp);
	m_pCamInfo->nRectH = _ttoi(sTemp);
	m_pCamInfo->nEffective = m_btnStart.GetCheck();

	CString sSql;
	sSql.Format(_T("UPDATE CamInfo SET CarParkName = '%s', GroupID = '%s', Effective = %d, BaseNum = %d, Offset = %d, RectX = %d, \
		RectY = %d, RectW = %d, RectH = %d WHERE PortID = %d"), m_pCamInfo->sCarParkName, m_pCamInfo->sGroupID, m_pCamInfo->nEffective, m_pCamInfo->nBaseNum
		, m_pCamInfo->nOffset, m_pCamInfo->nRectX, m_pCamInfo->nRectY, m_pCamInfo->nRectW, m_pCamInfo->nRectH, m_pCamInfo->nID);
	g_DB.ExecuteSQL((_bstr_t)sSql);

	CDialogEx::OnOK();
}