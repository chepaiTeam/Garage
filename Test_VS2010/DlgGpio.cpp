// DlgGpio.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "DlgGpio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGpio dialog


CDlgGpio::CDlgGpio(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGpio::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGpio)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGpio::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGpio)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGpio, CDialog)
	//{{AFX_MSG_MAP(CDlgGpio)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGpio message handlers
