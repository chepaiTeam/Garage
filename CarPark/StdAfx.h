
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


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
	int nLightColor;			//����ɫ��0 �ޣ�1 �죻2 ��
	int nBaseNum;				//������
	int nOffset;				//������ƫ����
	int nRectX;					//
	int nRectY;					//
	int nRectW;					//
	int nRectH;					//
} CAM_INFO, *pCAM_INFO;


#include "Database.h"
extern CDataBase g_DB;

extern bool g_bAppRun;								//�������б�־

extern CString g_sDeviceID;								//�豸���

extern int g_nNetType;								//�ÿ��Ƴ������������ NETTYPE_CLIENT:�ͻ���  NETTYPE_SERVER��������

extern CArray<CAM_INFO *, CAM_INFO *&> g_pCamInfos;							//�����Ϣ�ṹ��

extern CSerialPort* g_pPort1;							//����1
extern CSerialPort* g_pPort2;							//����2


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


