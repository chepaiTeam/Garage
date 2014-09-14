// StaticEx.cpp : 实现文件
//

#include "stdafx.h"
#include "CarPark.h"
#include "StaticEx.h"
#include "CarParkDlg.h"


// CStaticEx

IMPLEMENT_DYNAMIC(CStaticEx, CStatic)

CStaticEx::CStaticEx()
{

}

CStaticEx::~CStaticEx()
{
}


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CStaticEx 消息处理程序

BOOL CStaticEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SetCursor(LoadCursor(NULL, IDC_HAND));
	return TRUE;
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void CStaticEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( GetCapture() != this ){					//鼠标进入按钮
		SetCapture();
		((CCarParkDlg *)GetParent())->SetSelStc(this);
		Invalidate(FALSE);										//重绘按钮 
	}else{ 
		CRect rc; 
		this->GetClientRect( &rc ); 
		if ( !rc.PtInRect(point) ){							//鼠标离开按钮
			ReleaseCapture();
			((CCarParkDlg *)GetParent())->SetSelStc(NULL);
			Invalidate(FALSE);									//重绘按钮 
		} 
	} 
	CStatic::OnMouseMove(nFlags, point);
}
