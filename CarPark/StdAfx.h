
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


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
#include "VideoView.h"

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


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


