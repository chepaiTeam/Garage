// VideoView.cpp : implementation file
//

#include "stdafx.h"
#include "CarPark.h"
#include "VideoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoView

CVideoView::CVideoView()
{
	m_nIndex = -1;
}

CVideoView::~CVideoView()
{
}


BEGIN_MESSAGE_MAP(CVideoView, CStatic)
	//{{AFX_MSG_MAP(CVideoView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoView message handlers

void CVideoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nIndex>=0){
		AfxGetMainWnd()->PostMessage(WM_VIEW_DBCLICK,m_nIndex,(LPARAM)this);
	}
	CStatic::OnLButtonDblClk(nFlags, point);
}

void CVideoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nIndex>=0){
		AfxGetMainWnd()->PostMessage(WM_VIEW_CLICK,m_nIndex,(LPARAM)this);
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CVideoView::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	this->ModifyStyle(NULL,SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

void CVideoView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect rect;
	this->GetClientRect(&rect);
	//dc.FillSolidRect(&rect, RGB(1, 0, 0));

	CBitmap bmp1;
	HBITMAP bitmap;
	bitmap =(HBITMAP)::LoadImage(NULL, _T("bk.dll"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE|LR_DEFAULTSIZE);
	bmp1.Detach();
	bmp1.Attach(bitmap);

	BITMAP bm; 
	bmp1.GetBitmap(&bm);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = memDC.SelectObject(&bmp1);

	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	memDC.SelectObject(pOldBitmap);

	ReleaseDC(&dc); 

	// Do not call CStatic::OnPaint() for painting messages


}
