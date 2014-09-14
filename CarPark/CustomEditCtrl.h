#ifndef CUSTOMEDITCTRL_H
#define CUSTOMEDITCTRL_H

struct LTEDITCTRLNMHDR
{
   NMHDR hdr ;
} ;

#define  LT_EDITCTRL_NOTIFICATION_LOST_FOCUS    1
#define  BUFF_SIZE   100
#define  EDT_INTEGER 0
#define  EDT_DOUBLE 1

class CCustomEditCtrl : public CEdit
{
private:
   UINT   m_uType;
   UINT   m_uPrecent;
   DOUBLE m_dMax;
   DOUBLE m_dMin;

   
   LRESULT NotifyParent ( UINT uChildNotifyCode ) ;
   BOOL StringInRange (const TCHAR szText[]);
   BOOL ParseStringSuccess (const TCHAR pStr[]);
   VOID CCustomEditCtrl::GetLeftRightStrings (const TCHAR  szText[],
                                                TCHAR        szLeft[],
                                                TCHAR        szRight[]);
   VOID GetStringNotLast (TCHAR pOutStr[], const TCHAR pStr[]);
   
   INT CCustomEditCtrl::GetLengthSum (const TCHAR szFirst [],
                                        const TCHAR szSecond[],
                                        const TCHAR szThird []);

   VOID CCustomEditCtrl::GetStringNotFirst (TCHAR pOutStr[], const TCHAR pStr[]);
public:
   // Construction
   CCustomEditCtrl ( ) ;
   virtual ~CCustomEditCtrl();

   UINT GetType();
   BOOL SetType(UINT nValue);

   UINT GetPercent();
   BOOL SetPercent(UINT nValue);

   INT GetMaxRange();
   BOOL SetMaxRange(INT nValue);

   INT GetMinRange();
   BOOL SetMinRange(INT nValue);

  BOOL SetNumericEditBox (DOUBLE dMinRange,
                            DOUBLE dMaxRange,
                            DOUBLE dIncrement,
                            UINT   uPrecesion,
                            BOOL   bIsDouble);

 protected:
   afx_msg void    OnKillFocus ( CWnd* pNewWnd ) ; 
   afx_msg void    OnChar      ( UINT nChar, UINT nRepCnt, UINT nFlags ) ;

   DECLARE_MESSAGE_MAP()
};

#endif //CUSTOMEDITCTRL_H
