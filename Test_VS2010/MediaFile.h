// MediaFile.h: interface for the CMediaFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEDIAFILE_H__24D1E046_0BE5_487B_83FF_07A892AF5B5D__INCLUDED_)
#define AFX_MEDIAFILE_H__24D1E046_0BE5_487B_83FF_07A892AF5B5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMediaFile  
{
public:
	void IniFileHead(char *pbuf, int size);
	BOOL bWrite;
	BOOL CheckSize();
	unsigned int Write(char* pbuf, int size);
	unsigned int Seek(unsigned int size, input_seek_t method);
	void Close();
	BOOL Open(TCHAR *szFileName);
	CMediaFile();
	virtual ~CMediaFile();
	TCHAR szfilename[256];
private:
	FILE *file;
	DWORD dwsize;
	int m_Frame;
	CRITICAL_SECTION m_CritSec;	
	char szFileHead[200];
	int m_nFileHeadLength;
	BOOL m_bWriteFileHead;
};
#endif // !defined(AFX_MEDIAFILE_H__24D1E046_0BE5_487B_83FF_07A892AF5B5D__INCLUDED_)
