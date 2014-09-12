// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__056761CD_C48F_477F_903C_8531DE37EE4B__INCLUDED_)
#define AFX_STDAFX_H__056761CD_C48F_477F_903C_8531DE37EE4B__INCLUDED_

#if _MSC_VER > 501
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


typedef enum {

		INPUT_OPEN_ASCII,	
		INPUT_OPEN_BINARY

}input_mode_t;

typedef enum {

		INPUT_SEEK_SET,
		INPUT_SEEK_CUR,
		INPUT_SEEK_END

} input_seek_t;

typedef enum {

		INPUT_CREATE_READ,
		INPUT_CREATE_WRITE,
		INPUT_CREATE_READ_WRITE

} input_creat_mode_t;


#ifndef   _UNICODE   
#define   __stprintf         sprintf
#define   __tprintf          fprintf
#define   __vsnwprintf       _vsnprintf  
#define   __fopen            fopen  
#else   
#define   __stprintf       wsprintf  
#define   __tprintf        fwprintf 
#define   __vsnwprintf        _vsnwprintf
#define   __fopen           _wfopen
#endif

#define WM_VIEW_CLICK		WM_USER+100
#define WM_VIEW_DBCLICK		WM_USER+101
#define MAX_VIDEO_CHANNEL	36

#include "SerialPort.h"

#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv\cvaux.h>

#include "CvvImage.h"

#include "../API/datatype.h"
#include "../API/DvrsCap.h"
#include <vfw.h>

#pragma comment(lib, "Vfw32.lib")
#pragma comment(lib, "../lib/Dvrshw.lib")
//#pragma comment(lib, "../lib/JpegLib.lib")


#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
//#pragma comment(lib, "opencv_flann249.lib")
#pragma comment(lib, "opencv_highgui249.lib")
//#pragma comment(lib, "opencv_features2d249.lib")
//#pragma comment(lib, "opencv_calib3d249.lib")
//#pragma comment(lib, "opencv_objdetect249.lib")
//#pragma comment(lib, "opencv_video249.lib")
//#pragma comment(lib, "opencv_nonfree249.lib")
//#pragma comment(lib, "opencv_photo249.lib")
//#pragma comment(lib, "opencv_ml249.lib")
//#pragma comment(lib, "opencv_legacy249.lib")
//#pragma comment(lib, "opencv_gpu249.lib")
//#pragma comment(lib, "opencv_ts249.lib")


//#pragma comment(lib, "msimg32.lib")

typedef struct
{
	int nID;					//ID
	int nLightColor;			//灯颜色，0 无；1 红；2 绿
	int nBaseNum;				//检测基数
	int nOffset;				//检测基数偏移量
	int nRectX;					//
	int nRectY;					//
	int nRectW;					//
	int nRectH;					//
} CAM_INFO, *pCAM_INFO;


#include "Database.h"
extern CDataBase g_DB;

extern bool g_bAppRun;								//程序运行标志

extern CString g_sDeviceID;								//设备编号

extern int g_nNetType;								//该控制程序的网络类型 NETTYPE_CLIENT:客户端  NETTYPE_SERVER：服务器

extern CArray<CAM_INFO *, CAM_INFO *&> g_pCamInfos;							//相机信息结构体

extern CSerialPort* g_pPort1;							//串口1
extern CSerialPort* g_pPort2;							//串口2

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__056761CD_C48F_477F_903C_8531DE37EE4B__INCLUDED_)
