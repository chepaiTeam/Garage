// AviFileObj.cpp: implementation of the CAviFileObj class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AviFileObj.h"

#define ERR_INVALID_HWND	0x001

#define SET_ERR_CODE(p, ec)		{ if((p) != NULL) *(p) = (ec); };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAviFileObj::CAviFileObj()
{
	m_pAviFile = NULL;
	ZeroMemory(m_szFile, MAX_PATH*sizeof(TCHAR));
	
	m_psVideoStream = NULL;
	ZeroMemory(&m_siVideo, sizeof(AVISTREAMINFO));
	ZeroMemory(&m_bihVideo, sizeof(BITMAPINFOHEADER));
	m_lVideoStart = 0;
	m_lVideoSamples = 0;

	m_dwAudioFormat = -1;
	m_psAudioStream = NULL;
	ZeroMemory(&m_siAudio, sizeof(AVISTREAMINFO));
	m_lpAudioFormat = NULL;
	m_lAudioStart = 0;
	m_lAudioSamples = 0;

	AVIFileInit();

	InitializeCriticalSection(&m_csObject);
}

CAviFileObj::~CAviFileObj()
{
	Close(NULL);

	AVIFileExit();

	DeleteCriticalSection(&m_csObject);
}

// 创建之前一定要先设置格式
HRESULT CAviFileObj::Create(TCHAR* szFile, UINT uiMode, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = E_FAIL;

	if(m_dwAudioFormat == -1 || m_siVideo.fccType == 0)
	{
		hr = E_INVALIDARG;

		SET_ERR_CODE(pdwError, hr);

		LeaveCriticalSection(&m_csObject);
		
		return hr;
	}

	// 打开文件
	hr = AVIFileOpen(&m_pAviFile, szFile, uiMode, NULL);
	if(FAILED(hr))
	{
		SET_ERR_CODE(pdwError, hr);

		LeaveCriticalSection(&m_csObject);

		return hr;
	}

	// 创建视频流
	hr = AVIFileCreateStream(m_pAviFile, &m_psVideoStream, &m_siVideo);
	if(SUCCEEDED(hr))
	{
		// 设置格式
		hr = AVIStreamSetFormat(m_psVideoStream, 0, (PVOID)&m_bihVideo, sizeof(BITMAPINFOHEADER));
	}
	if(FAILED(hr))
	{
		SET_ERR_CODE(pdwError, hr);

		LeaveCriticalSection(&m_csObject);

		Close(NULL);

		return hr;
	}

	//
	hr = AVIFileCreateStream(m_pAviFile, &m_psAudioStream, &m_siAudio);
	if(SUCCEEDED(hr))
	{
		// 设置格式
		switch(m_dwAudioFormat)
		{
		case 0: // adpcm, 8000Hz, mono, 4bits
			hr = AVIStreamSetFormat(m_psAudioStream, 0, (LPVOID)m_lpAudioFormat, sizeof(IMAADPCMWAVEFORMAT));
			break;
		default:
			hr = E_INVALIDARG;
			break;
		}
	}
	if(FAILED(hr))
	{
		SET_ERR_CODE(pdwError, hr);

		LeaveCriticalSection(&m_csObject);

		Close(NULL);

		return hr;
	}

	wsprintf(m_szFile, _T("%s"), szFile);
// 	CopyMemory(m_szFile, szFile, _tcslen(szFile)*sizeof(TCHAR));
	
	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);
	
	return hr;
}
HRESULT CAviFileObj::Close(DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = E_FAIL;

	if(m_psAudioStream != NULL)
	{
		hr = AVIStreamEndStreaming(m_psAudioStream);
		hr = AVIStreamRelease(m_psAudioStream);
	}
	m_psAudioStream = NULL;
	m_dwAudioFormat = -1;
	ZeroMemory(&m_siAudio, sizeof(AVISTREAMINFO));
	if(m_lpAudioFormat)
	{
     delete m_lpAudioFormat;
	m_lpAudioFormat = NULL;
	}
	m_lAudioStart = 0;
	m_lAudioSamples = 0;

	if(m_psVideoStream != NULL)
	{
		hr = AVIStreamEndStreaming(m_psVideoStream);
		hr = AVIStreamRelease(m_psVideoStream);
	}
	m_psVideoStream = NULL;
	ZeroMemory(&m_siVideo, sizeof(AVISTREAMINFO));
	ZeroMemory(&m_bihVideo, sizeof(BITMAPINFOHEADER));
	m_lVideoStart = 0;
	m_lVideoSamples = 0;

	if(m_pAviFile)
	{
		hr = AVIFileEndRecord(m_pAviFile);
		hr = AVIFileRelease(m_pAviFile);
	}
	m_pAviFile = NULL;

	ZeroMemory(m_szFile, MAX_PATH*sizeof(TCHAR));

	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);

	return hr;
}

HRESULT CAviFileObj::SetVideoFormat(FOURCC fccVideo, DWORD dwWidth, DWORD dwHeight, DWORD dwRate, DWORD dwScale, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = S_OK;

	if(fccVideo == 0 || dwWidth == 0 || dwHeight == 0 || dwScale == 0)
	{
		hr = E_INVALIDARG;

		SET_ERR_CODE(pdwError, hr);

		LeaveCriticalSection(&m_csObject);

		return hr;
	}

	if(m_pAviFile != NULL) // 文件已经打开了，不能更改格式
	{
		SET_ERR_CODE(pdwError, hr);

		LeaveCriticalSection(&m_csObject);

		return hr;
	}

	// avi stream info
	m_siVideo.fccType = fccVideo;
	m_siVideo.fccHandler = mmioFOURCC('X','V','I','D');
	
	m_siVideo.dwRate = dwRate;
	m_siVideo.dwScale = dwScale;

	m_siVideo.rcFrame.left = 0;
	m_siVideo.rcFrame.top = 0;
	m_siVideo.rcFrame.right = dwWidth;
	m_siVideo.rcFrame.bottom = dwHeight;
	m_siVideo.dwSuggestedBufferSize = dwWidth*dwHeight*2; // YUY2

	m_siVideo.dwQuality = -1; // 0 - 10000

	// image format
	m_bihVideo.biSize		= sizeof(BITMAPINFOHEADER);
	m_bihVideo.biCompression = m_siVideo.fccHandler;
	m_bihVideo.biWidth		= m_siVideo.rcFrame.right-m_siVideo.rcFrame.left;
	m_bihVideo.biHeight		= m_siVideo.rcFrame.bottom-m_siVideo.rcFrame.top;
	m_bihVideo.biBitCount	= 16; // for YUY2
	m_bihVideo.biPlanes		= 1;
	m_bihVideo.biSizeImage	= m_siVideo.dwSuggestedBufferSize;

	m_lVideoSamples = 1; // 每次写一个采样

	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);

	return hr;
}
HRESULT CAviFileObj::GetVideoFormat(FOURCC& fccVideo, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwRate, DWORD& dwScale, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = S_OK;

	m_siVideo.fccType = MAKEFOURCC('v','i','d','s');
	fccVideo = m_siVideo.fccHandler;
	
	dwRate = m_siVideo.dwRate;
	dwScale = m_siVideo.dwScale;

	dwWidth = m_siVideo.rcFrame.right - m_siVideo.rcFrame.left;
	dwHeight = m_siVideo.rcFrame.bottom - m_siVideo.rcFrame.top;

	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);

	return hr;
}

HRESULT CAviFileObj::SetAudioFormat(DWORD dwFormat, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = S_OK;

	switch(dwFormat)
	{
	case 0: // ADPCM, 8000Hz, mono, 4bits
		{
			m_dwAudioFormat = 0;
			m_dwAudioFormat = 0;

			// avi stream info
			m_siAudio.fccType				= MAKEFOURCC('a','u','d','s');
			m_siAudio.dwRate				= 1600;
			m_siAudio.dwScale				= 101;
			m_siAudio.dwSuggestedBufferSize = 0;
			m_siAudio.dwSampleSize			= 168;

			// audio format
			m_lpAudioFormat = (LPVOID) new IMAADPCMWAVEFORMAT;
			ZeroMemory(m_lpAudioFormat, sizeof(IMAADPCMWAVEFORMAT));
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.wFormatTag		= WAVE_FORMAT_IMA_ADPCM;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.nChannels		= 1;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.nSamplesPerSec	= 8000;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.nAvgBytesPerSec	= 4055;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.nBlockAlign		= 256;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.wBitsPerSample	= 4;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wfx.cbSize			= 2;
			((IMAADPCMWAVEFORMAT*)m_lpAudioFormat)->wSamplesPerBlock	= 505;

			//
			m_lAudioSamples = 1; // 每次写一个采样
		}
		break;

	default:
		m_dwAudioFormat = -1;
		hr = E_INVALIDARG;
		break;
	}

	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);

	return hr;
}
HRESULT CAviFileObj::GetAudioFormat(DWORD& dwFormat, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = S_OK;

	dwFormat = m_dwAudioFormat;

	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);

	return hr;
}

HRESULT CAviFileObj::WriteVideo(BYTE* pbData, LONG lSize, BOOL bIsKeyFrame, BOOL bPaused, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = S_OK;

	if(m_psVideoStream == NULL || m_pAviFile == NULL)
	{
		hr = E_INVALIDARG;

		SET_ERR_CODE(pdwError, hr);

		return hr;
	}

	if(!bPaused)
	{
		hr = AVIStreamWrite(m_psVideoStream, m_lVideoStart++, m_lVideoSamples, pbData, lSize, bIsKeyFrame?AVIIF_KEYFRAME:0, NULL, NULL);
	}
	else
	{
		hr = S_OK;
		m_lVideoStart++;
	}


	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);

	return hr;
}

HRESULT CAviFileObj::WriteAudio(BYTE* pbData, LONG lSize, BOOL bPaused, DWORD* pdwError)
{
	EnterCriticalSection(&m_csObject);

	HRESULT hr = S_OK;

	if(m_psAudioStream == NULL || m_pAviFile == NULL)
	{
		hr = E_INVALIDARG;

		SET_ERR_CODE(pdwError, hr);

		return hr;
	}

	if(!bPaused)
	{
		hr = AVIStreamWrite(m_psAudioStream, m_lAudioStart++, m_lAudioSamples, pbData, lSize, 0, NULL, NULL);
 		//m_lAudioStart += lSize / m_siAudio.dwSampleSize;
	}
	else
	{
		hr = S_OK;
		m_lAudioSamples++;
	}

	SET_ERR_CODE(pdwError, hr);

	LeaveCriticalSection(&m_csObject);
	
	return hr;
}
