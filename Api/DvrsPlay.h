////////////////////////////////////////////////////////////////////////////////
//DvrsPlay.h
//////////////////////////////////////////////////////////////////////////////

#ifndef _DVRS_GVPlay_SDK_H_
#define _DVRS_GVPlay_SDK_H_

#include "datatype.h"

#ifndef  DVRS_CAP_EXPORT
#define DVRS_API __declspec(dllexport)
#endif DVRS_CAP_EXPORT

# ifdef __cplusplus
extern "C"
{
# endif 
//Max channel numbers
#define PLAY_MAX_SUPPORTS		128


#define FILEMODE       1
#define STREAMMODE     2
#define STREAMFILEMODE 3
//Frame position
typedef struct{
	long nFilePos;
	long nFrameNum;
	long nFrameTime;
        ULONG	FrameNumber;
        USHORT	Length;
        BYTE  FrameType;
	BYTE	Magic;
	UNION_INFO info1;
	UNION_INFO info2;
}FRAME_POS,*PFRAME_POS;
//Frame Info
typedef struct
{
	long nWidth;
	long nHeight;
	long nStamp;
	long nType;
	long nFrameRate;
} FRAME_INFO;
#define T_UYVY					1
#define T_YV12					3
#define T_RGB32					7
#define T_RGB24					8

typedef void (CALLBACK *DEC_CALLBACK)(long nChannel,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);
typedef void (CALLBACK *DISPLAY_CALLBACK) (long nChannel,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved);
typedef void (CALLBACK *FILEREF_CALLBACK) (DWORD nChannel,DWORD nUser);
DVRS_API BOOL  __stdcall Play_InitDDraw(HWND hWnd);
DVRS_API BOOL  __stdcall Play_RealeseDDraw();
DVRS_API BOOL  __stdcall Play_OpenFile(LONG nChannel,TCHAR *sFileName);
DVRS_API BOOL  __stdcall Play_CloseFile(LONG nChannel);
DVRS_API BOOL  __stdcall Play_Play(LONG nChannel, HWND hWnd);
DVRS_API BOOL  __stdcall Play_Stop(LONG nChannel);
DVRS_API BOOL  __stdcall Play_Pause(LONG nChannel,DWORD nPause);
DVRS_API BOOL  __stdcall Play_Fast(LONG nChannel);
DVRS_API BOOL  __stdcall Play_Slow(LONG nChannel);
DVRS_API BOOL  __stdcall Play_OneByOne(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetPlayPos(LONG nChannel,DWORD fRelativePos);
DVRS_API DWORD __stdcall Play_GetPlayPos(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetFileEndMsg(LONG nChannel,HWND hWnd,UINT nMsg);
DVRS_API BOOL  __stdcall Play_SetVolume(LONG nChannel,WORD nVolume);
DVRS_API BOOL  __stdcall Play_StopSound();
DVRS_API BOOL  __stdcall Play_PlaySound(LONG nChannel);
DVRS_API BOOL  __stdcall Play_OpenStream(LONG nChannel,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);
DVRS_API BOOL  __stdcall Play_InputData(LONG nChannel,PBYTE pBuf,DWORD nSize);
DVRS_API BOOL  __stdcall Play_CloseStream(LONG nChannel);
DVRS_API int   __stdcall Play_GetCaps();
DVRS_API DWORD __stdcall Play_GetFileTime(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetPlayedTime(LONG Number,DWORD Time);
DVRS_API DWORD __stdcall Play_GetPlayedTime(LONG nChannel);
DVRS_API DWORD __stdcall Play_GetPlayedFrames(LONG nChannel);
DVRS_API BOOL  __stdcall Play_CaptureImage(LONG nChannel,char *FileName);
DVRS_API DWORD __stdcall Play_GetFileTotalFrames(LONG nChannel);
DVRS_API DWORD __stdcall Play_GetCurrentFrameRate(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetCurrentFrameNum(LONG nChannel,DWORD nFrameNum);
DVRS_API BOOL  __stdcall Play_OneByOneBack(LONG nChannel,int framerate);
DVRS_API BOOL  __stdcall Play_SetStreamOpenMode(LONG nChannel,DWORD nMode);
DVRS_API LONG  __stdcall Play_GetStreamOpenMode(LONG nChannel);
DVRS_API BOOL  __stdcall Play_GetPictureSize(LONG nChannel,LONG *pWidth,LONG *pHeight);
DVRS_API WORD  __stdcall Play_GetVolume(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetPicQuality(LONG nChannel,BOOL bHighQuality);
DVRS_API BOOL  __stdcall Play_BackFast(LONG nChannel, int iSpeed, int iStep);
DVRS_API DWORD __stdcall Play_GetLastError(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetBackFastEndMsg(LONG nChannel, HWND hWnd, UINT nMsg);
DVRS_API BOOL  __stdcall Play_SetPlayPosEx( LONG nChannel,float fRelativePos);
DVRS_API BOOL  __stdcall Play_SetDecCallBack(LONG nChannel,void (CALLBACK* DecCBFun)(long nChannel,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2));
DVRS_API BOOL  __stdcall Play_SetDisplayCallBack(LONG nChannel,void (CALLBACK* DisplayCBFun)(long nChannel,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved));
DVRS_API BOOL  __stdcall Play_SetFileRefCallBack(LONG nChannel, void (__stdcall *pFileRefDone)(DWORD nChannel,DWORD nUser), DWORD nUser );
DVRS_API BOOL  __stdcall Play_RefreshPlay(LONG nChannel);
DVRS_API DWORD  __stdcall Play_GetSdkVersion();
DVRS_API BOOL  __stdcall Play_SetDecCallBack(LONG nChannel,DEC_CALLBACK dec_callback);
DVRS_API BOOL  __stdcall Play_SetFileRefCallBack(LONG nChannel, FILEREF_CALLBACK fileref_callback, DWORD nUser );
DVRS_API BOOL  __stdcall Play_SetBackFastEndMsg(LONG nChannel, HWND hWnd, UINT nMsg);
DVRS_API BOOL  __stdcall Play_SetPicQuality(LONG nChannel,BOOL bHighQuality);
DVRS_API BOOL  __stdcall Play_BackFast(LONG nChannel, int iSpeed, int iStep);
DVRS_API DWORD __stdcall Play_GetLastError(LONG nChannel);
DVRS_API BOOL  __stdcall Play_SetSearchIndex(LONG nChannel);
DVRS_API  DWORD __stdcall Play_GetFileTimeEx(TCHAR *sFileName);
# ifdef __cplusplus
}
# endif

#endif