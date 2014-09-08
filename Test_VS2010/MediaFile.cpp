// MediaFile.cpp: implementation of the CMediaFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "test.h"
#include "MediaFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMediaFile::CMediaFile()
{
   file = NULL;
   dwsize = 0;
   bWrite = FALSE;
   m_Frame = 0;
   memset(szFileHead,0,sizeof(szFileHead));
   m_nFileHeadLength = 0;
   m_bWriteFileHead = FALSE;
   InitializeCriticalSection(&m_CritSec);
}

CMediaFile::~CMediaFile()
{
    Close();
	DeleteCriticalSection(&m_CritSec);
}

BOOL CMediaFile::Open(TCHAR *szFileName)
{
	EnterCriticalSection(&m_CritSec);
	if(this->file!= NULL)
	{
		fflush(this->file);
		fclose(this->file);
		this->file = NULL;
		dwsize = 0;
		memset(szfilename,0,sizeof(szfilename));
		bWrite = FALSE;
	}
    this->file = __fopen(szFileName, _T("w+b"));
	
	if(this->file == NULL) {
		LeaveCriticalSection(&m_CritSec);
		return FALSE;
	}
	__stprintf(szfilename,szFileName);
	this->Seek(0, INPUT_SEEK_END);
	this->dwsize = this->Seek(0, INPUT_SEEK_CUR);
	this->Seek(0, INPUT_SEEK_SET);
	m_Frame = 0;
	LeaveCriticalSection(&m_CritSec);
	return TRUE;

}

void CMediaFile::Close()
{
	EnterCriticalSection(&m_CritSec);
	if(!this->file)
	{
		LeaveCriticalSection(&m_CritSec);
		return ;
	}
    fflush(this->file);
	fclose(this->file);
    this->file = NULL;
	dwsize = 0;
	memset(szfilename,0,sizeof(szfilename));
	bWrite = FALSE;
	m_bWriteFileHead = FALSE;
	LeaveCriticalSection(&m_CritSec);
}

unsigned int CMediaFile::Seek(unsigned int size, input_seek_t method)
{
	if(!this->file)
		return 0;

	switch(method) {

		case INPUT_SEEK_SET:
			return fseek(this->file, size, SEEK_SET);
			break;			
		case INPUT_SEEK_CUR:
			if(size == 0) {
				return ftell(this->file);	
			}
			else {
				return fseek(this->file, size, SEEK_CUR);
			}
			break;
		case INPUT_SEEK_END:
			return fseek(this->file, size, SEEK_END);
			break;
	}

	return 0;
}

unsigned int CMediaFile::Write(char *pbuf, int size)
{
	EnterCriticalSection(&m_CritSec);
	if(!pbuf || !this->file || size<=0 ||!bWrite)
	{
		LeaveCriticalSection(&m_CritSec);
		return -1;
	}
	unsigned int nResult = 0;
    dwsize+=size;
	m_Frame++;
	if(m_Frame>50)
	{
	 // fflush(this->file);
	  m_Frame =0;
	}
    if(!m_bWriteFileHead && m_nFileHeadLength>0)
	{
	   m_bWriteFileHead = true;
	   fwrite(szFileHead, 1, m_nFileHeadLength, this->file);
	}
	nResult = fwrite(pbuf, 1, size, this->file);
	LeaveCriticalSection(&m_CritSec);
	return nResult;
}

BOOL CMediaFile::CheckSize()
{
    if(dwsize<10*1024)
	{
	  return FALSE;
	}
	else
	{
	  return TRUE;
	}
}

void CMediaFile::IniFileHead(char *pbuf, int size)
{
   if(size<=200)
   {
	   memcpy(szFileHead,pbuf,size);
	   m_nFileHeadLength = size;
   }

}
