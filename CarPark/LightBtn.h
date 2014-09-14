#pragma once


// CLightBtn

class CLightBtn : public CMFCButton
{
	DECLARE_DYNAMIC(CLightBtn)

public:
	CLightBtn();
	virtual ~CLightBtn();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	Image *m_pImgLightRed;		//ºìµÆ
	Image *m_pImgLightGreen;	//ÂÌµÆ
	Image *m_pImgLightRG;		//ºìÂÌµÆ
	Image *m_pImgLightNone;		//Ï¨Ãð

	int m_nLightType;

public:
	void SetLightPNG(Image *pRed, Image *pGreen, Image *pRG, Image *pNone);
	void SetLightType(int nLightType);
	int GetLightType();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();
};


