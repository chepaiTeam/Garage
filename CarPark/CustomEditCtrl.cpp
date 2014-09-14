#include "Stdafx.h"
#include "CustomEditCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BEGIN_MESSAGE_MAP ( CCustomEditCtrl, CEdit ) 
ON_WM_KILLFOCUS ( ) 
ON_WM_CHAR ( )
END_MESSAGE_MAP ( ) 


CCustomEditCtrl::CCustomEditCtrl ( ) 
{
   m_uType    = EDT_DOUBLE;
   m_uPrecent = 0;
   m_dMax     = 0;
   m_dMin     = 0;
}

CCustomEditCtrl::~CCustomEditCtrl ( ) 
{
}

LRESULT CCustomEditCtrl::NotifyParent ( UINT uChildNotifyCode ) 
{
   LTEDITCTRLNMHDR EditHdr ;
   CWnd            *pWndParent ;
   UINT            uID ;
   LRESULT         retMsg ;

   retMsg = 0 ;

   pWndParent = GetParent ( ) ;

   if ( NULL != pWndParent ) 
   {     
      uID = ( UINT ) ::GetWindowLong ( this->m_hWnd, GWL_ID ) ;

      EditHdr.hdr.hwndFrom = m_hWnd ;

      EditHdr.hdr.idFrom  = uID ;

      EditHdr.hdr.code = uChildNotifyCode ;

      retMsg = pWndParent->SendMessage ( WM_NOTIFY, ( WPARAM ) uID, ( LPARAM ) &EditHdr ) ;
   }

   return retMsg ;
}

void CCustomEditCtrl::OnKillFocus ( CWnd* pNewWnd ) 
{
   NotifyParent ( LT_EDITCTRL_NOTIFICATION_LOST_FOCUS ) ;

   CWnd::OnKillFocus ( pNewWnd ) ;
}


BOOL CCustomEditCtrl::StringInRange (const TCHAR szText[])
{
   DOUBLE    dVal;
   BOOL      bRet;
   INT       nVal;

   if (m_uType == EDT_DOUBLE)
   {
      dVal = _tcstod (szText, NULL);

      if ((m_dMax >= dVal) && (m_dMin <= dVal))
         bRet = TRUE;
      else
         bRet = FALSE;
   }
   else
   {
      nVal = _ttoi (szText);

      if (((INT)(m_dMax) >= nVal) && ((INT)(m_dMin) <= nVal))
         bRet = TRUE;
      else
         bRet = FALSE;
   }
   return bRet;
}

BOOL CCustomEditCtrl::ParseStringSuccess (const TCHAR pStr[])
{
   UINT   uCtr       = 0;
   UINT   uCtrPrec   = 0;
   BOOL   bFoundDot  = FALSE;
   BOOL   bCheckPrec = FALSE;

   if ((pStr[uCtr] == '+') || (pStr[uCtr] == '-'))
      uCtr++;

   while (pStr[uCtr])
   {
      if (pStr[uCtr] == '.')
      {
         if (m_uType == EDT_DOUBLE)
         {
            if (!bFoundDot)
            {
               bCheckPrec = TRUE;
               bFoundDot  = TRUE;
            }
            else
               return FALSE;
         }
         else
         {
            return FALSE;
         }
      }
      else
      {
         if (!((pStr[uCtr] >= '0') && (pStr[uCtr] <= '9')))
            return FALSE;
      }

      if (bCheckPrec && (uCtrPrec++ > m_uPrecent))
            return FALSE;

      uCtr ++;
   }
   return TRUE;
}


VOID CCustomEditCtrl::GetLeftRightStrings (const TCHAR  szText[],
                                             TCHAR        szLeft[],
                                             TCHAR        szRight[])
{
   INT   uCurselStrt, uCurselEnd;
   INT   uLStrIdx, uRStrIdx, uCtr;

   GetSel(uCurselStrt, uCurselEnd);
   
   for (uLStrIdx = 0; uLStrIdx < uCurselStrt; uLStrIdx++)
      szLeft[uLStrIdx] = szText[uLStrIdx];

   uRStrIdx = 0;
   uCtr = uCurselEnd;
   while (TRUE)
   {
      if (!szText[uCtr])
         break;
      szRight[uRStrIdx++] = szText[uCtr++];
   }
}

VOID CCustomEditCtrl::GetStringNotLast (TCHAR pOutStr[], const TCHAR pStr[])
{
   UINT   nCtr;
   
   if (!pStr[0])
   {
      pOutStr[0] = '\0';
      return;
   }

   nCtr = 0;
   while (TRUE)
   {
      if (!pStr[nCtr + 1])
      {
         pOutStr[nCtr] = '\0';
         break;
      }

      pOutStr[nCtr] = pStr[nCtr];
      nCtr++;
   }
}

INT CCustomEditCtrl::GetLengthSum (const TCHAR szFirst [],
                                     const TCHAR szSecond[],
                                     const TCHAR szThird [])
{
   INT nSum = 0;

   if (szFirst)
      nSum += lstrlen (szFirst);

   if (szSecond)
      nSum += lstrlen (szSecond);

   if (szThird)
      nSum += lstrlen (szThird);

   return ++nSum;
}

VOID CCustomEditCtrl::GetStringNotFirst (TCHAR pOutStr[], const TCHAR pStr[])
{
   UINT   nCtr;

   nCtr = 0;
   while (TRUE)
   {
      if (!pStr[nCtr])
      {
         pOutStr[nCtr] = '\0';
         break;
      }

      pOutStr[nCtr] = pStr[nCtr + 1];
      nCtr++;
   }
}

void CCustomEditCtrl::OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags )
{
   CString  strText;
   INT    uCurselStrt, uCurselEnd;
   TCHAR    szLeft [BUFF_SIZE] = TEXT("\0");
   TCHAR    szRight[BUFF_SIZE] = TEXT("\0");
   TCHAR    szTemp [BUFF_SIZE] = TEXT("\0");
   TCHAR    szText [BUFF_SIZE] = TEXT("\0");
   BOOL   bInsertOn;

   if (nChar == VK_BACK)
   {
      GetWindowText(strText);
      INT nLength = strText.GetLength() * sizeof(TCHAR);

      memcpy(szText, strText.GetBuffer(nLength), nLength);
      GetSel (uCurselStrt, uCurselEnd);
      GetLeftRightStrings (szText, szLeft, szRight);

      if (uCurselStrt == uCurselEnd)
      {
         GetStringNotLast (szTemp, szLeft);
         wsprintf (szLeft, TEXT("%s"), szTemp);
      }

      wsprintf (szText, TEXT("%s%s"), szLeft, szRight);
      if (StringInRange(szText))
      {
         CEdit::OnChar ( nChar, nRepCnt, nFlags );
         return;
      }
      else
         return ;
   }

   if ((nChar >= 0) && (nChar <= 29))
   {
      CEdit::OnChar ( nChar, nRepCnt, nFlags );
      return;
   }


   memset (szLeft,  0, BUFF_SIZE);
   memset (szRight, 0, BUFF_SIZE);
   memset (szText, 0, BUFF_SIZE);

   GetWindowText(strText);
   memcpy(szText, strText.GetBuffer(strText.GetLength() * sizeof(TCHAR)), strText.GetLength() * sizeof(TCHAR));

   GetSel(uCurselStrt, uCurselEnd);
   GetLeftRightStrings(szText, szLeft, szRight);

   if (uCurselEnd - uCurselStrt)
   {
      // There is a selection
      if (nChar == VK_BACK)
      {
         wsprintf (szText, TEXT("%s%s"), szLeft, szRight);
      }
      else
      {
         if (GetLengthSum (szLeft, szRight, TEXT("x")) > BUFF_SIZE)
            return;
         else
            wsprintf (szText, TEXT("%s%c%s"), szLeft, nChar, szRight);
      }
   }
   else
   {
      // No selection
      if (nChar == VK_BACK)
      {
         GetStringNotLast (szTemp, szLeft);
         wsprintf (szText, TEXT("%s%s"), szTemp, szRight);
      }
      else
      {
         bInsertOn = GetKeyState (VK_INSERT) & 1;
         if (bInsertOn)
         {
            GetStringNotFirst (szTemp, szRight);
            wsprintf (szText, TEXT("%s%c%s"), szLeft, nChar, szTemp);
         }
         else
         {
            if (GetLengthSum (szLeft, szRight, TEXT("x")) > BUFF_SIZE)
               return ;
            else
               wsprintf (szText, TEXT("%s%c%s"), szLeft, nChar, szRight);
         }
      }
   }


   if (!ParseStringSuccess(szText))
         return ;

   if (StringInRange(szText))
   {
      CEdit::OnChar ( nChar, nRepCnt, nFlags );
   }
   return ;
}

UINT CCustomEditCtrl::GetType()
{
   return m_uType;
}

BOOL CCustomEditCtrl::SetType(UINT nValue)
{
   m_uType = nValue;
   return TRUE;
}

UINT CCustomEditCtrl::GetPercent()
{
   return m_uPrecent;
}

BOOL CCustomEditCtrl::SetPercent(UINT nValue)
{
   m_uPrecent = nValue;
   return TRUE;
}

INT CCustomEditCtrl::GetMaxRange()
{
   return (INT)m_dMax;
}

BOOL CCustomEditCtrl::SetMaxRange(INT nValue)
{
   m_dMax = nValue;
   return TRUE;
}

INT CCustomEditCtrl::GetMinRange()
{
   return (INT)m_dMin;
}
   
BOOL CCustomEditCtrl::SetMinRange(INT nValue)
{
   m_dMin = nValue;
   return TRUE;
}

BOOL CCustomEditCtrl::SetNumericEditBox (DOUBLE dMinRange,
                                          DOUBLE dMaxRange,
                                          DOUBLE dIncrement,
                                          UINT   uPrecesion,
                                          BOOL   bIsDouble)
{
   UNREFERENCED_PARAMETER(dIncrement);
   m_uType    = bIsDouble ? EDT_DOUBLE : EDT_INTEGER;
   m_uPrecent = uPrecesion;
   m_dMax     = dMaxRange;
   m_dMin     = dMinRange;

   return TRUE;
}