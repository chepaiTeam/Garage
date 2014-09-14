// LightBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "CarPark.h"
#include "LightBtn.h"


// CLightBtn

IMPLEMENT_DYNAMIC(CLightBtn, CMFCButton)

CLightBtn::CLightBtn()
{
	m_nLightType = LIGHT_TYPE_NONE;
	m_pImgLightRed = NULL;
	m_pImgLightGreen = NULL;
	m_pImgLightRG = NULL;
	m_pImgLightNone = NULL;
}

CLightBtn::~CLightBtn()
{
}


BEGIN_MESSAGE_MAP(CLightBtn, CMFCButton)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CLightBtn 消息处理程序

void CLightBtn::SetLightPNG(Image *pRed, Image *pGreen, Image *pRG, Image *pNone)
{
	m_pImgLightRed = pRed;
	m_pImgLightGreen = pGreen;
	m_pImgLightRG = pRG;
	m_pImgLightNone = pNone;
}

void CLightBtn::SetLightType(int nLightType)
{
	m_nLightType = nLightType;
}

int CLightBtn::GetLightType()
{
	return m_nLightType;
}

void CLightBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	CRect rect;
	GetWindowRect(rect);

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	MemDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	MemDC.SelectObject(&MemBitmap);

	Graphics graphics(MemDC.m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	COLORREF dwClr = GetPixel(GetParent()->GetDC()->m_hDC,3,330);
	SolidBrush solidBrush(Color(GetRValue(dwClr),GetGValue(dwClr),GetBValue(dwClr)));
	graphics.FillRectangle(&solidBrush, -1, -1, rect.Width() + 1, rect.Height() + 1);

	switch(m_nLightType)
	{
	case LIGHT_TYPE_RED:
		graphics.DrawImage(m_pImgLightRed, 0, 0, m_pImgLightRed->GetWidth(), m_pImgLightRed->GetHeight());
		break;
	case LIGHT_TYPE_GREEN:
		graphics.DrawImage(m_pImgLightGreen, 0, 0, m_pImgLightGreen->GetWidth(), m_pImgLightGreen->GetHeight());
		break;
	case LIGHT_TYPE_RG:
		graphics.DrawImage(m_pImgLightRG, 0, 0, m_pImgLightRG->GetWidth(), m_pImgLightRG->GetHeight());
		break;
	case LIGHT_TYPE_NONE:
		graphics.DrawImage(m_pImgLightNone, 0, 0, m_pImgLightNone->GetWidth(), m_pImgLightNone->GetHeight());
		break;
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
	MemDC.DeleteDC();
	MemBitmap.DeleteObject();
}


void CLightBtn::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(BS_DEFPUSHBUTTON, BS_OWNERDRAW);
	CMFCButton::PreSubclassWindow();
}
