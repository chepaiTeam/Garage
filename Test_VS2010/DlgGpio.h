#if !defined(AFX_DLGGPIO_H__0045328A_CCD6_44FE_8B9D_093CA825F9EB__INCLUDED_)
#define AFX_DLGGPIO_H__0045328A_CCD6_44FE_8B9D_093CA825F9EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGpio.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGpio dialog

class CDlgGpio : public CDialog
{
// Construction
public:
	CDlgGpio(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGpio)
	enum { IDD = IDD_DIALOG_GPIO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGpio)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGpio)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGPIO_H__0045328A_CCD6_44FE_8B9D_093CA825F9EB__INCLUDED_)
