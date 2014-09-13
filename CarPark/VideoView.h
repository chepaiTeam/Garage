#if !defined(AFX_VIDEOVIEW_H__34C7C5D2_5269_46C9_8F87_93B18A0B1EC1__INCLUDED_)
#define AFX_VIDEOVIEW_H__34C7C5D2_5269_46C9_8F87_93B18A0B1EC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVideoView window

class CVideoView : public CStatic
{
// Construction
public:
	CVideoView();

// Attributes
public:
    int m_nIndex;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoView)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVideoView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVideoView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOVIEW_H__34C7C5D2_5269_46C9_8F87_93B18A0B1EC1__INCLUDED_)
