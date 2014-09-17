// ServerSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarPark.h"
#include "ServerSetDlg.h"
#include "afxdialogex.h"
#include "Server.h"


// CServerSetDlg 对话框

IMPLEMENT_DYNAMIC(CServerSetDlg, CDialogEx)

CServerSetDlg::CServerSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerSetDlg::IDD, pParent)
{
	m_bListAction = TRUE;
	m_bIsChange = FALSE;
}

CServerSetDlg::~CServerSetDlg()
{
	g_bLedChange = m_bIsChange;
}

void CServerSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listLedInfo);
	DDX_Control(pDX, IDC_EDIT1, m_edtAddress);
	DDX_Control(pDX, IDC_COMBO1, m_cboDirection);
	DDX_Control(pDX, IDC_EDIT3, m_edtCarNum);
	DDX_Control(pDX, IDC_LIST2, m_boxCarGroup);
	DDX_Control(pDX, IDC_EDIT2, m_edtCarGroup);
	DDX_Control(pDX, IDC_LIST3, m_boxTextFormat);
	DDX_Control(pDX, IDC_EDIT8, m_edtTextFormatMin);
	DDX_Control(pDX, IDC_EDIT11, m_edtTextFormatMax);
	DDX_Control(pDX, IDC_COMBO2, m_cboTextFormatColor);
	DDX_Control(pDX, IDC_EDIT9, m_edtTagFormatMin);
	DDX_Control(pDX, IDC_EDIT12, m_edtTagFormatMax);
	DDX_Control(pDX, IDC_COMBO3, m_cboTagFormatColor);
	DDX_Control(pDX, IDC_LIST4, m_boxTagFormat);
	DDX_Control(pDX, IDC_COMBO4, m_cboComName);
	DDX_Control(pDX, IDC_EDIT7, m_edtLedName);
}


BEGIN_MESSAGE_MAP(CServerSetDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CServerSetDlg::OnLvnItemchangedList1)
	ON_LBN_SELCHANGE(IDC_LIST3, &CServerSetDlg::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST4, &CServerSetDlg::OnLbnSelchangeList4)
	ON_LBN_SELCHANGE(IDC_LIST2, &CServerSetDlg::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON4, &CServerSetDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CServerSetDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CServerSetDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CServerSetDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CServerSetDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CServerSetDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON1, &CServerSetDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CServerSetDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CServerSetDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON10, &CServerSetDlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// CServerSetDlg 消息处理程序


BOOL CServerSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listLedInfo.ModifyStyle(0, LVS_REPORT);
	m_listLedInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listLedInfo.InsertColumn(0, _T("LED屏编号"), LVCFMT_LEFT, 60);
	m_listLedInfo.InsertColumn(1, _T("LED地 址"), LVCFMT_LEFT, 60);
	m_listLedInfo.InsertColumn(2, _T("车 位 组"), LVCFMT_LEFT, 100);
	m_listLedInfo.InsertColumn(3, _T("方    向"), LVCFMT_LEFT, 50);
	m_listLedInfo.InsertColumn(4, _T("车位总数"), LVCFMT_LEFT, 60);
	m_listLedInfo.InsertColumn(5, _T("文字格式"), LVCFMT_LEFT, 150);
	m_listLedInfo.InsertColumn(6, _T("标签格式"), LVCFMT_LEFT, 150);
	m_listLedInfo.InsertColumn(7, _T("串口地址"), LVCFMT_LEFT, 60);

	_RecordsetPtr rs = g_DB.GetRecordset(_T("SELECT * FROM LEDInfo"));
	for(int i = 0; !rs->adoEOF; i++)
	{
		m_listLedInfo.InsertItem(i, "");
		CString sTemp;
		if (rs->GetCollect(_T("Address")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 1, (LPCSTR)(_bstr_t)rs->GetCollect(_T("Address")));
		}

		if (rs->GetCollect(_T("LEDName")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 0, (LPCSTR)(_bstr_t)rs->GetCollect(_T("LEDName")));
		}

		if (rs->GetCollect(_T("Groups")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 2, (LPCSTR)(_bstr_t)rs->GetCollect(_T("Groups")));
		}

		if (rs->GetCollect(_T("Direction")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 3, (LPCSTR)(_bstr_t)rs->GetCollect(_T("Direction")));
		}

		if (rs->GetCollect(_T("ParkingLotNum")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 4, (LPCSTR)(_bstr_t)rs->GetCollect(_T("ParkingLotNum")));
		}

		if (rs->GetCollect(_T("TextFormat")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 5, (LPCSTR)(_bstr_t)rs->GetCollect(_T("TextFormat")));
		}

		if (rs->GetCollect(_T("TagFormat")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 6, (LPCSTR)(_bstr_t)rs->GetCollect(_T("TagFormat")));
		}

		if (rs->GetCollect(_T("ComNum")).vt != VT_NULL)
		{
			m_listLedInfo.SetItemText(i, 7, (LPCSTR)(_bstr_t)rs->GetCollect(_T("ComNum")));
		}
		rs->MoveNext();
	}
	rs->Close();

	m_edtCarNum.SetNumericEditBox(0, 20000, 1, 0, FALSE);

	m_edtTextFormatMin.SetNumericEditBox(0, 20000, 1, 0, FALSE);
	m_edtTextFormatMax.SetNumericEditBox(0, 20000, 1, 0, FALSE);

	m_edtTagFormatMin.SetNumericEditBox(0, 20000, 1, 0, FALSE);
	m_edtTagFormatMax.SetNumericEditBox(0, 20000, 1, 0, FALSE);

	m_cboComName.AddString(_T("1"));
	m_cboComName.AddString(_T("2"));
	m_cboComName.SetCurSel(0);

	m_cboDirection.AddString(_T("L"));
	m_cboDirection.AddString(_T("R"));
	m_cboDirection.AddString(_T("U"));
	m_cboDirection.AddString(_T("D"));
	m_cboDirection.SetCurSel(0);

	m_cboTextFormatColor.AddString(_T("R"));
	m_cboTextFormatColor.AddString(_T("G"));
	m_cboTextFormatColor.AddString(_T("B"));
	m_cboTextFormatColor.SetCurSel(0);

	m_cboTagFormatColor.AddString(_T("R"));
	m_cboTagFormatColor.AddString(_T("G"));
	m_cboTagFormatColor.AddString(_T("B"));
	m_cboTagFormatColor.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CServerSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg-> message>=WM_KEYFIRST && pMsg-> wParam<=WM_KEYLAST) 
	{ 
		if(pMsg->wParam == VK_RETURN || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	} 
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CServerSetDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION   pos = m_listLedInfo.GetFirstSelectedItemPosition();
	if(pos == NULL || !m_bListAction)
	{return;}
	else 
	{
		CString sTem;
		CString s1, s2;
		int nS1 = 0, nS2 = 0;

		int nIdx = m_listLedInfo.GetNextItem(-1, LVIS_SELECTED);
		sTem = m_listLedInfo.GetItemText(nIdx, 0);
		m_edtLedName.SetWindowText(sTem);

		sTem = m_listLedInfo.GetItemText(nIdx, 1);
		m_edtAddress.SetWindowText(sTem);

		sTem = m_listLedInfo.GetItemText(nIdx, 2);
		m_boxCarGroup.ResetContent();
		m_edtCarGroup.SetWindowText(_T(""));
		if (sTem != "")
		{
			nS1 = 0;
			while(AfxExtractSubString(s1, sTem, nS1, ';'))
			{
				m_boxCarGroup.AddString(s1);
				nS1++;
			}
		}

		sTem = m_listLedInfo.GetItemText(nIdx, 3);
		m_cboDirection.SelectString(0, sTem);

		sTem = m_listLedInfo.GetItemText(nIdx, 4);
		m_edtCarNum.SetWindowText(sTem);

		sTem = m_listLedInfo.GetItemText(nIdx, 5);
		m_boxTextFormat.ResetContent();
		m_edtTextFormatMin.SetWindowText(_T(""));
		m_edtTextFormatMax.SetWindowText(_T(""));
		m_cboTextFormatColor.SetCurSel(0);
		if (sTem != "")
		{
			nS1 = 0;
			while(AfxExtractSubString(s1, sTem, nS1, ';'))
			{
				m_boxTextFormat.AddString(s1);
				nS1++;
			}
		}

		sTem = m_listLedInfo.GetItemText(nIdx, 6);
		m_boxTagFormat.ResetContent();
		m_edtTagFormatMin.SetWindowText(_T(""));
		m_edtTagFormatMax.SetWindowText(_T(""));
		m_cboTagFormatColor.SetCurSel(0);
		if (sTem != "")
		{
			nS1 = 0;
			while(AfxExtractSubString(s1, sTem, nS1, ';'))
			{
				m_boxTagFormat.AddString(s1);
				nS1++;
			}
		}

		sTem = m_listLedInfo.GetItemText(nIdx, 7);
		m_cboComName.SelectString(0, sTem);
	}
	*pResult = 0;
}

void CServerSetDlg::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_boxCarGroup.GetCurSel();
	if (nSel < 0)
		return;

	CString sTem;
	m_boxCarGroup.GetText(nSel, sTem);
	m_edtCarGroup.SetWindowText(sTem);
}

//增加
void CServerSetDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTem;
	m_edtCarGroup.GetWindowText(sTem);
	if (sTem == "")
		return;

	int nIndex = m_boxCarGroup.AddString(sTem);
	m_boxCarGroup.SetCurSel(nIndex);

	int a = m_boxCarGroup.GetCount();
	m_edtCarGroup.SetWindowText("");
}

//移除
void CServerSetDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_boxCarGroup.GetCurSel();
	if(nSel < 0)
		return;

	m_boxCarGroup.DeleteString(nSel);
	m_edtCarGroup.SetWindowText(_T(""));
}


//文字格式
void CServerSetDlg::OnLbnSelchangeList3()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_boxTextFormat.GetCurSel();
	if (nSel < 0)
		return;

	CString sTem;
	m_boxTextFormat.GetText(nSel, sTem);
	CString s1, s2;
	int nS1 = 0, nS2 = 0;
	CString sMin, sMax;
	CString sRgb;
	while(AfxExtractSubString(s1, sTem, nS1, ';'))
	{
		while(AfxExtractSubString(s2, s1, nS2, ':'))
		{
			switch(nS2)
			{
			case 0:
				sMin = s2;
				break;
			case 1:
				sMax = s2;
				break;
			case 2:
				sRgb = s2;
				break;
			}
			nS2++;
		}
		if (nS2 == 3)
		{
			m_edtTextFormatMin.SetWindowText(sMin);
			m_edtTextFormatMax.SetWindowText(sMax);
			m_cboTextFormatColor.SelectString(0, sRgb);
		}
		nS2 = 0;
		nS1++;
	}
}

void CServerSetDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sMin, sMax;
	CString sRgb;

	m_edtTextFormatMin.GetWindowText(sMin);
	m_edtTextFormatMax.GetWindowText(sMax);
	m_cboTextFormatColor.GetWindowText(sRgb);

	CString sTem;
	sTem.Format(_T("%s:%s:%s"), sMin, sMax, sRgb);
	int nIndex = m_boxTextFormat.AddString(sTem);
	m_boxTextFormat.SetCurSel(nIndex);

	m_edtTextFormatMin.SetWindowText("");
	m_edtTextFormatMax.SetWindowText("");
	m_cboTextFormatColor.SetCurSel(0);
}


void CServerSetDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_boxTextFormat.GetCurSel();
	if(nSel < 0)
		return;

	m_boxTextFormat.DeleteString(nSel);
	m_edtTextFormatMin.SetWindowText(_T(""));
	m_edtTextFormatMax.SetWindowText(_T(""));
	m_cboTextFormatColor.SetCurSel(0);
}


void CServerSetDlg::OnLbnSelchangeList4()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_boxTagFormat.GetCurSel();
	if (nSel < 0)
		return;

	CString sTem;
	m_boxTagFormat.GetText(nSel, sTem);
	CString s1, s2;
	int nS1 = 0, nS2 = 0;
	CString sMin, sMax;
	CString sRgb;
	while(AfxExtractSubString(s1, sTem, nS1, ';'))
	{
		while(AfxExtractSubString(s2, s1, nS2, ':'))
		{
			switch(nS2)
			{
			case 0:
				sMin = s2;
				break;
			case 1:
				sMax = s2;
				break;
			case 2:
				sRgb = s2;
				break;
			}
			nS2++;
		}
		if (nS2 == 3)
		{
			m_edtTagFormatMin.SetWindowText(sMin);
			m_edtTagFormatMax.SetWindowText(sMax);
			m_cboTagFormatColor.SelectString(0, sRgb);
		}
		nS2 = 0;
		nS1++;
	}
}


void CServerSetDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sMin, sMax;
	CString sRgb;

	m_edtTagFormatMin.GetWindowText(sMin);
	m_edtTagFormatMax.GetWindowText(sMax);
	m_cboTagFormatColor.GetWindowText(sRgb);

	CString sTem;
	sTem.Format(_T("%s:%s:%s"), sMin, sMax, sRgb);
	int nIndex = m_boxTagFormat.AddString(sTem);
	m_boxTagFormat.SetCurSel(nIndex);

	m_edtTagFormatMin.SetWindowText("");
	m_edtTagFormatMax.SetWindowText("");
	m_cboTagFormatColor.SetCurSel(0);
}


void CServerSetDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_boxTagFormat.GetCurSel();
	if(nSel < 0)
		return;

	m_boxTagFormat.DeleteString(nSel);
	m_edtTagFormatMin.SetWindowText(_T(""));
	m_edtTagFormatMax.SetWindowText(_T(""));
	m_cboTagFormatColor.SetCurSel(0);
}

//保存
void CServerSetDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bListAction = FALSE;

	CString sTem;
	CString sLedName, sAddress, sCarGroups, sDirection, sTextFormat, sTagFormat;
	int nCarNum = 0, nComName = 0;
	CString s1, s2;
	int nS1 = 0, nS2 = 0;

	m_edtAddress.GetWindowText(sAddress);
	sTem.Format(_T("SELECT * FROM LEDInfo WHERE Address = '%s'"), sAddress);
	_RecordsetPtr rs = g_DB.GetRecordset((_bstr_t)sTem);
	if (!rs->adoEOF)
	{
		m_bListAction = TRUE;
		rs->Close();
		MessageBox(_T("该LED地址已存在！"));
		return;
	}
	rs->Close();

	int nItem = m_listLedInfo.InsertItem(m_listLedInfo.GetItemCount(), "");

	m_edtLedName.GetWindowText(sLedName);
	m_listLedInfo.SetItemText(nItem, 0, sLedName);

	m_listLedInfo.SetItemText(nItem, 1, sAddress);

	for (int i = 0; i < m_boxCarGroup.GetCount(); i++)
	{
		m_boxCarGroup.GetText(i, sTem);
		if (sTem != "")
		{
			if (i == 0)
			{
				sCarGroups = sTem;
			}else
			{
				sCarGroups += _T(";") + sTem;
			}
		}
	}
	m_listLedInfo.SetItemText(nItem, 2, sCarGroups);

	m_cboDirection.GetWindowText(sDirection);
	m_listLedInfo.SetItemText(nItem, 3, sDirection);

	m_edtCarNum.GetWindowText(sTem);
	nCarNum = _ttoi(sTem);
	m_listLedInfo.SetItemText(nItem, 4, sTem);

	for (int i = 0; i < m_boxTextFormat.GetCount(); i++)
	{
		m_boxTextFormat.GetText(i, sTem);
		if (sTem != "")
		{
			if (i == 0)
			{
				sTextFormat = sTem;
			}else
			{
				sTextFormat += _T(";") + sTem;
			}
		}
	}
	m_listLedInfo.SetItemText(nItem, 5, sTextFormat);

	for (int i = 0; i < m_boxTagFormat.GetCount(); i++)
	{
		m_boxTagFormat.GetText(i, sTem);
		if (sTem != "")
		{
			if (i == 0)
			{
				sTagFormat = sTem;
			}else
			{
				sTagFormat += _T(";") + sTem;
			}
		}
	}
	m_listLedInfo.SetItemText(nItem, 6, sTagFormat);

	m_cboComName.GetWindowText(sTem);
	nComName = _ttoi(sTem);
	m_listLedInfo.SetItemText(nItem, 7, sTem);

	sTem.Format(_T("INSERT INTO LEDInfo VALUES('%s', '%s', '%s', '%s', %d, '%s', '%s', %d)")
		,sLedName, sAddress, sCarGroups, sDirection, nCarNum, sTextFormat, sTagFormat, nComName);
	g_DB.ExecuteSQL((_bstr_t)sTem);

	POSITION   pos = m_listLedInfo.GetFirstSelectedItemPosition();
	while(pos)
	{
		m_listLedInfo.SetItemState(m_listLedInfo.GetNextSelectedItem(pos), 0, -1);
	}
	m_listLedInfo.EnsureVisible(nItem,   FALSE);   //滚动条自动滚动到第ItemCount行 
	//m_ListView.SetExtendedStyle(   LVS_EX_FULLROWSELECT   );     //设置高亮显示属性 
	m_listLedInfo.SetFocus();     //set   list   focus 
	m_listLedInfo.SetItemState(nItem,   LVIS_SELECTED,   LVIS_SELECTED);   //选中行

	m_bListAction = TRUE;
	m_bIsChange = TRUE;
}

//修改
void CServerSetDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bListAction = FALSE;

	CString sTem;
	CString sLedName, sAddress, sCarGroups, sDirection, sTextFormat, sTagFormat;
	int nCarNum = 0, nComName = 0;
	CString s1, s2;
	int nS1 = 0, nS2 = 0;

	m_edtAddress.GetWindowText(sAddress);
	sTem.Format(_T("SELECT * FROM LEDInfo WHERE Address = '%s'"), sAddress);
	_RecordsetPtr rs = g_DB.GetRecordset((_bstr_t)sTem);
	if (rs->adoEOF)
	{
		m_bListAction = TRUE;
		rs->Close();
		MessageBox(_T("该LED地址不存在！"));
		return;
	}
	rs->Close();
	int nItem = -1;
	for (int i = 0; i < m_listLedInfo.GetItemCount(); i++)
	{
		sTem = m_listLedInfo.GetItemText(i, 1);
		if (sTem == sAddress)
		{
			nItem = i;
			break;
		}
	}
	m_edtLedName.GetWindowText(sLedName);
	m_listLedInfo.SetItemText(nItem, 0, sLedName);

	m_listLedInfo.SetItemText(nItem, 1, sAddress);

	for (int i = 0; i < m_boxCarGroup.GetCount(); i++)
	{
		m_boxCarGroup.GetText(i, sTem);
		if (sTem != "")
		{
			if (i == 0)
			{
				sCarGroups = sTem;
			}else
			{
				sCarGroups += _T(";") + sTem;
			}
		}
	}
	m_listLedInfo.SetItemText(nItem, 2, sCarGroups);

	m_cboDirection.GetWindowText(sDirection);
	m_listLedInfo.SetItemText(nItem, 3, sDirection);

	m_edtCarNum.GetWindowText(sTem);
	nCarNum = _ttoi(sTem);
	m_listLedInfo.SetItemText(nItem, 4, sTem);

	for (int i = 0; i < m_boxTextFormat.GetCount(); i++)
	{
		m_boxTextFormat.GetText(i, sTem);
		if (sTem != "")
		{
			if (i == 0)
			{
				sTextFormat = sTem;
			}else
			{
				sTextFormat += _T(";") + sTem;
			}
		}
	}
	m_listLedInfo.SetItemText(nItem, 5, sTextFormat);

	for (int i = 0; i < m_boxTagFormat.GetCount(); i++)
	{
		m_boxTagFormat.GetText(i, sTem);
		if (sTem != "")
		{
			if (i == 0)
			{
				sTagFormat = sTem;
			}else
			{
				sTagFormat += _T(";") + sTem;
			}
		}
	}
	m_listLedInfo.SetItemText(nItem, 6, sTagFormat);

	m_cboComName.GetWindowText(sTem);
	nComName = _ttoi(sTem);
	m_listLedInfo.SetItemText(nItem, 7, sTem);

	sTem.Format(_T("UPDATE LEDInfo SET LEDName = '%s', Groups = '%s', Direction = '%s', \
				   ParkingLotNum = %d, TextFormat = '%s', TagFormat = '%s', ComNum = %d WHERE Address = '%s'")
				   ,sLedName, sCarGroups, sDirection, nCarNum, sTextFormat, sTagFormat, nComName, sAddress);
	g_DB.ExecuteSQL((_bstr_t)sTem);

	m_bListAction = TRUE;
	m_bIsChange = TRUE;
}

//删除
void CServerSetDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTem;
	CString sAddress;

	int nIdx = m_listLedInfo.GetNextItem(-1, LVIS_SELECTED);
	if (nIdx < 0)
		return;

	sAddress = m_listLedInfo.GetItemText(nIdx, 1);
	sTem.Format(_T("是否删除LED地址为 %s 的信息？"), sAddress);

	if(MessageBox(sTem, _T("提示"), MB_YESNO) == IDYES)
	{
		m_listLedInfo.DeleteItem(nIdx);
		sTem.Format(_T("DELETE FROM LEDInfo WHERE Address = '%s'"), sAddress);
		g_DB.ExecuteSQL(_bstr_t(sTem));
		m_bIsChange = TRUE;
	}
}


void CServerSetDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
