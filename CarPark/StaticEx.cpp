// StaticEx.cpp : ʵ���ļ�
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



// CStaticEx ��Ϣ�������

BOOL CStaticEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SetCursor(LoadCursor(NULL, IDC_HAND));
	return TRUE;
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void CStaticEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( GetCapture() != this ){					//�����밴ť
		SetCapture();
		((CCarParkDlg *)GetParent())->SetSelStc(this);
		Invalidate(FALSE);										//�ػ水ť 
	}else{ 
		CRect rc; 
		this->GetClientRect( &rc ); 
		if ( !rc.PtInRect(point) ){							//����뿪��ť
			ReleaseCapture();
			((CCarParkDlg *)GetParent())->SetSelStc(NULL);
			Invalidate(FALSE);									//�ػ水ť 
		} 
	} 
	CStatic::OnMouseMove(nFlags, point);
}
