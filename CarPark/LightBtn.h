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
	Image *m_pImgLightRed;		//���
	Image *m_pImgLightGreen;	//�̵�
	Image *m_pImgLightRG;		//���̵�
	Image *m_pImgLightNone;		//Ϩ��

	int m_nLightType;

public:
	void SetLightPNG(Image *pRed, Image *pGreen, Image *pRG, Image *pNone);
	void SetLightType(int nLightType);
	int GetLightType();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();
};


