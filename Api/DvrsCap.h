////////////////////////////////////////////////////////////////////////////////
//DvrsCap.h
//////////////////////////////////////////////////////////////////////////////

#ifndef _DVRS_CAP_SDK_H_
#define _DVRS_CAP_SDK_H_

#include "datatype.h"

//预览窗口排列方式定义(主要是为overlay方式用)
typedef struct _PREVIEWRC
{
	int   sizeUnit;				//本窗口的大小倍数（以标准单位计）
	int   xUnit, yUnit;			//左上坐标位置（以标准单位计）
} stPreviewRC, *pstPreviewRC; 
typedef struct _PREVIEWMODE
{
	int nPreviewCount;			//窗口总数
	int hUnit, vUnit;			//x, y方向上的标准单位数目
	int nWidth, nHeight;		//标准预览的宽和高(overlay)
	int maxWidth, maxHeight;	//最大预览的宽和高(offscreen)
	stPreviewRC* nPreviewPos;	//预览排列的大小及位置信息数组指针(至少包含窗口总数个)
} stPreviewMode, *pstPreviewMode;

#ifndef  DVRS_CAP_EXPORT
#define DVRS_API __declspec(dllexport)
#endif DVRS_CAP_EXPORT



#define ERR_NONE							0
#define ERR_CHANNEL							0xc0000001

# ifdef __cplusplus
extern "C"
{
# endif 



typedef void (*DRAWFUN_CALLBACK)(long nPort, HDC hDc, LONG nUser, DWORD dwReserved);
typedef void (*LOGRECORD_CALLBACK)(char *str, void *context);

/// <summary>
/// 直接读取码流回调函数
/// </summary>
/// <param name="channelNumber">通道号</param>
/// <param name="context">设备上下文</param>
/// <returns></returns>
typedef int (*STREAM_READ_CALLBACK)(ULONG channelNumber, void *context);
typedef int (*VIDEO_STATUS_CALLBACK)(ULONG channelNumber, BOOL bStatus, void *context);

/// <summary>
/// 编码数据流直接读取回调函数
/// </summary>
/// <param name="channelNumber">通道号</param>
/// <param name="DataBuf">缓冲区地址</param>
/// <param name="Length">缓冲区长度</param>
/// <param name="FrameType">缓冲区数据帧类型</param>
/// <param name="context">设备上下文</param>
/// <returns></returns>
typedef int (*STREAM_DIRECT_READ_CALLBACK)(ULONG channelNumber, void *DataBuf, DWORD Length, int FrameType, void *context);


DVRS_API int __stdcall Test(HANDLE hChannel);
/// 1.板卡初始化及卸载

/// <summary>
/// 1.1初始化DSP InitDSPs
///     说  明： 初始化系统中每一块板卡，应在应用软件程序启动时完成。如果返回值为0则表
///     明初始化失败，可能没有找到相应的DSP软件模块。
/// 
/// </summary>
/// <returns>系统内可用的编码通道个数。 </returns>
DVRS_API int __stdcall InitDSPs();

/// <summary>
/// 1.2卸载DSP DeInitDSPs
///     说  明：  关闭每一块板卡上的功能，应在应用软件程序退出时调用。 
/// 
/// int __stdcall DeInitDSPs()
/// </summary>
/// <returns>0</returns>
DVRS_API int __stdcall DeInitDSPs();
DVRS_API HANDLE __stdcall ChannelOpen(int nChannel);
DVRS_API int __stdcall ChannelClose(HANDLE hChannel);

/// <summary>
/// 2.3获取系统中编码通道的个数
///     说  明：  获取系统中所有编码卡的编码通道总个数，包含H系列和HC系列编码卡。
/// 
/// </summary>
/// <returns>系统中编码通道的个数</returns>
DVRS_API int __stdcall GetTotalChannels();

/// <summary>
/// 2.2获取系统中DSP的个数
///     说  明：  获取系统中所有板卡的DSP的个数。 
/// 
/// </summary>
/// <returns>系统中DSP的总个数</returns>
DVRS_API int __stdcall GetTotalDSPs();

DVRS_API int __stdcall StopAllDSPs();
DVRS_API int __stdcall ReStartAllDSPs();
DVRS_API int __stdcall StartDsp(HANDLE hChannel);
DVRS_API int __stdcall StopDsp(HANDLE hChannel);

//////////////////////////////////////////////
// preview 
////////////////////////////////////////////////////////

/// <summary> 
/// SDK自3.2版本起在部分显卡中实现了HC卡以overlay方式预览的功能（此功 
/// 能不支持与H卡混插的状态下），可以提高预览的画质和降低CPU利用率。当预览画面小 
/// 于704*576时，需要调用该函数来启动overlay模式，如不设置则自动切换到offscreen模式 
/// 进行预览显示，当预览画面大于704*576时，SDK自动切换到overlay模式 
/// </summary> 
/// <param name="bTrue">是否设置overlay预览方式，也适用于MD卡</param> 
/// <returns>0表示显卡支持板卡的overlay预览方式；其他值表示显卡不支持</returns> 
DVRS_API int __stdcall SetPreviewOverlayMode(BOOL bTrue);

/// <summary> 
/// 启动视频预览，调用SetPreviewOverlayMode后，可进行overlay模式预览， 
/// 否则，将默认采用offscreen模式预览。当画面大于704*576时，SDK自动切换到overlay预览模式。 
/// </summary> 
/// <param name="hChannelHandle"></param> 
/// <param name="WndHandle"></param> 
/// <param name="rect"></param> 
/// <param name="bOverlay"></param> 
/// <param name="VideoFormat"></param> 
/// <param name="FrameRate"></param> 
/// <returns>成功返回0；失败返回错误号</returns> 
DVRS_API int __stdcall StartVideoPreview(HANDLE hChannel, HWND WndHandle, RECT *rect, BOOL bOverlay, int VideoFormat, int FrameRate);

/// <summary> 
/// 停止视频预览 
/// </summary> 
/// <param name="hChannelHandle">通道句柄</param> 
/// <returns>成功返回0；失败返回错误号</returns> 
DVRS_API int __stdcall StopVideoPreview(HANDLE hChannel);

/// <summary> 
/// 板卡在显示范围小于704*576时，调用SetPreviewOverlayMode可以开启overlay 
/// 预览模式，需调用SetOverlayColorKey设置overlay关键色；当显示范围大于704*576时， 
/// 板卡自动切换到overlay预览模式，关键色默认设置为RGB（10，10，10），也可调用 
/// SetOverlayColorKey修改关键色。在这两种情况下，都需要将显示窗口的底色设置为和关键 
/// 色相一致。否则图像将难以显示。 
/// </summary> 
/// <param name="DestColorKey">overlay关键色参数（RGB（*，*，*））</param> 
/// <returns>成功返回0；失败返回错误号</returns> 
DVRS_API int __stdcall SetOverlayColorKey(COLORREF DestColorKey);


/// <summary> 
/// 设置视频参数 
/// </summary> 
/// <param name="hChannelHandle">通道句柄</param> 
/// <param name="Brightness">亮度值（0-255）</param> 
/// <param name="Contrast">对比度（0-127）</param> 
/// <param name="Saturation">饱和度（0-127）</param> 
/// <param name="Hue">色调（0-255）</param> 
/// <returns>成功返回0；失败返回错误号</returns> 
DVRS_API int __stdcall SetVideoPara(HANDLE hChannel, int Brightness, int Contrast, int Saturation, int Hue);

/// <summary> 
/// 获取视频参数 
/// </summary> 
/// <param name="hChannelHandle">通道句柄</param> 
/// <param name="VideoStandard">视频制式 
/// StandardNone；无视频信号 0x80000000 
/// StandardNTSC；NTSC制式 0x00000001 
/// StandardPAL；PAL制式 0x00000002</param> 
/// <param name="Brightness">亮度指针值（0-255）</param> 
/// <param name="Contrast">对比度指针值（0-127）</param> 
/// <param name="Saturation">饱和度指针值（0-127）</param> 
/// <param name="Hue">色调指针值（0-255）</param> 
/// <returns>成功返回0；失败返回错误号</returns> 
DVRS_API int __stdcall GetVideoPara(HANDLE hChannel, VideoStandard_t *VideoStandard, int *Brightness, int *Contrast, int *Saturation, int *Hue);

/// <summary>
/// 3.3.4获取视频信号输入情况GetVideoSignal
///     说  明：  获取视频信号的输入情况，用于视频丢失报警 
/// 
/// int __stdcall GetVideoSignal(HANDLE  hChannelHandle)
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <returns>信号正常返回0；返回其他值说明信号异常或有错误</returns>
DVRS_API int __stdcall GetVideoSignal(HANDLE  hChannel);

/// <summary>
/// 3.4.4.3.1设置主通道分辨率SetEncodERPictureFormat
///     说 明：  设置主通道编码分辨率。支持动态修改。
/// 
/// int __stdcall SetEncoderPictureFormat(HANDLE hChannelHandle,  PictureFormat_t PictureFormat) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="PictureFormat">编码图像分辨率（4CIF、DCIF、2CIF、CIF、QCIF） </param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall SetEncoderPictureFormat(HANDLE hChannel, PictureFormat_t PictureFormat);

/// <summary>
/// 3.3.1设置视频制式SetVideoStandard，此函数只对H卡有效
///     说  明：  设置视频制式，在某一制式的摄像头已经接好的情况下启动系统时可不必调用该
///     函数，如果没有接摄像头的情况下启动系统然后再接NTSC制式的摄像头则必须调用该函    
///     数，或者中途调换不同制式的摄像头也必须调用该函数。
/// 
/// int __stdcall SetVideoStandard(HANDLE hChannelHandle,  VideoStandard_t VideoStandard) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="VideoStandard">视频制式</param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall SetVideoStandard(HANDLE hChannel, VideoStandard_t VideoStandard);
DVRS_API int __stdcall StartVideoCapture(HANDLE hChannel);
DVRS_API int __stdcall StopVideoCapture(HANDLE hChannel);
DVRS_API int __stdcall ConfigSubChannelSplit( int nMaxSubCh, int nMode=0 );

/// <summary>
/// 3.4.3.1设置主通道编码流类型SetStreamType
///     说  明：  设置主通道编码流类型。此函数需在启动编码前进行设置
/// 
/// int __stdcall SetStreamType(HANDLE hChannelHandle, USHORT Type) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="Type">流类型</param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall SetStreamType(HANDLE hChannel, USHORT Type);

/// 3.4.3.2获取主通道编码流类型GetStreamType
///     说  明：  获取主通道编码流类型
/// 
/// int __stdcall GetStreamType(HANDLE hChannelHandle, USHORT *StreamType) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="StreamType">流类型</param>
/// <returns>成功返回0；失败返回错误号 </returns>
DVRS_API int __stdcall GetStreamType(HANDLE hChannel, USHORT *StreamType);

/// <summary>
/// 2.8获取板卡型号及序列号信息GetBoardInfo
///     说  明：  获取板卡的型号及序列号信息
/// 
/// int __stdcall GetBoardInfo(HANDLE hChannelHandle, ULONG *BoardType,  UCHAR *SerialNo)
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="BoardType">板卡型号</param>
/// <param name="SerialNo">
/// 板卡ID号, 内容为板卡序列号的ASCII的数值，次序为SerialNo[0] 对应最高位，
/// SerialNo[11]对应最低位。比如卡号为“40000002345”的值对应为 4,0,0,0,0,1,0,0,2,3,4,5 的整形数组。
/// </param>
/// <returns>成功为0；失败返回错误号 </returns>
DVRS_API int __stdcall GetBoardInfo(HANDLE hChannel, ULONG *BoardType, UCHAR *SerialNo);

DVRS_API int __stdcall RegisterLogRecordCallback(LOGRECORD_CALLBACK LogRecordFunc, void *Context);

/// <summary>
/// 3.5.3.2读取码流函数
///     说 明：  读指定长度的数据流，适用于方式二及方式三。当调用StartVideoCapture 或
///     StartMotionDetection后，SDK线程会向已注册的用户窗口消息处理函数发送指定的消息，
///     并提供消息来源的通道号。当用户程序收到该消息时，可调用本函数来读取数据，Length 在
///     作为输入时必须提供缓冲的大小，ReadStreamData会判断缓冲是否足够，如果缓冲足够大
///     则返回值为当前的读取的帧长度，否则返回错误。
///         在HC卡中，如果已经先调用了RegisterStreamDirectReadCallback()函数，则不需调用 
///     ReadStreamData来读取数据，因为音视频数据会在RegisterStreamDirectReadCallback所注册
///     的回调函数中直接返回。
/// 
/// int __stdcall ReadStreamData(HANDLE hChannelHandle, void *DataBuf,  DWORD *Length, int *FrameType) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="DataBuf">自定义的数据缓存区</param>
/// <param name="Length">输入：缓存区的大小；输出：一帧数据的大小</param>
/// <param name="FrameType">帧类型</param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall ReadStreamData(HANDLE hChannel, void *DataBuf, DWORD *Length, int *FrameType);

/// <summary>
/// 3.5.3.1.2.2注册消息读取码流函数
///     说 明：  当数据准备好时，SDK会向hWnd窗口发送MessageId消息，目标窗口收到
///     Message后调用ReadStreamData读取一帧数据。如果HC卡与H卡混插，可以先调用 
///     RegisterStreamDirectReadCallback函数来注册HC卡取码流回调函数，再调用
///     RegisterMessageNotifyHandle函数来注册H卡取码流消息函数。
///     HC系列板卡建议使用方式一进行数据捕获。 
/// 
/// int __stdcall RegisterMessageNotifyHandle(HWND hWnd, UINT MessageId)
/// </summary>
/// <param name="hWnd">通道句柄</param>
/// <param name="MessageId">自定义消息</param>
/// <returns>成功返回0；失败返回错误号 </returns>
DVRS_API int __stdcall RegisterMessageNotifyHandle(HWND hWnd, UINT MessageId);

/// <summary>
/// 3.5.3.1.3.1注册直接读取码流回调函数
///     说 明：  另一种数据流读取方式
/// 
/// int __stdcall RegisterStreamReadCallback (STREAM_READ_CALLBACK StreamReadCallback, void *Context) 
/// </summary>
/// <param name="StreamReadCallback">直接读取码流回调函数</param>
/// <param name="Context">设备上下文</param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall RegisterStreamReadCallback(STREAM_READ_CALLBACK StreamReadCallback, void *Context);

/// <summary>
/// 3.5.3.2读取码流函数
///     说 明：  读指定长度的数据流，适用于方式二及方式三。当调用StartVideoCapture 或
///     StartMotionDetection后，SDK线程会向已注册的用户窗口消息处理函数发送指定的消息，
///     并提供消息来源的通道号。当用户程序收到该消息时，可调用本函数来读取数据，Length 在
///     作为输入时必须提供缓冲的大小，ReadStreamData会判断缓冲是否足够，如果缓冲足够大
///     则返回值为当前的读取的帧长度，否则返回错误。
///         在HC卡中，如果已经先调用了RegisterStreamDirectReadCallback()函数，则不需调用 
///     ReadStreamData来读取数据，因为音视频数据会在RegisterStreamDirectReadCallback所注册
///     的回调函数中直接返回。
/// 
/// int __stdcall ReadStreamData(HANDLE hChannelHandle, void *DataBuf,  DWORD *Length, int *FrameType) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="DataBuf">自定义的数据缓存区</param>
/// <param name="Length">输入：缓存区的大小；输出：一帧数据的大小</param>
/// <param name="FrameType">帧类型</param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall RegisterStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);
DVRS_API int __stdcall RegisterSubStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);
DVRS_API int __stdcall RegisterSubQCIFStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);

/// <summary>
/// 3.4.5强制设定I帧CaptureIFrame
///     说 明：  将当前编码帧强制设定为I帧模式，可从码流中提取该帧单独用于网络传送。
/// 
/// int __stdcall CaptureIFrame(HANDLE hChannelHandle)
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall CaptureIFrame(HANDLE hChannel);

/// <summary>
/// 3.4.4.3.1设置主通道分辨率SetEncodERPictureFormat
///     说 明：  设置主通道编码分辨率。支持动态修改。
/// 
/// int __stdcall SetEncoderPictureFormat(HANDLE hChannelHandle,  PictureFormat_t PictureFormat) 
/// </summary>
/// <param name="hChannelHandle">通道句柄</param>
/// <param name="PictureFormat">编码图像分辨率（4CIF、DCIF、2CIF、CIF、QCIF） </param>
/// <returns>成功返回0；失败返回错误号</returns>
DVRS_API int __stdcall SetEncoderPictureFormat(HANDLE hChannel, PictureFormat_t PictureFormat);

///////////////////////////
//  capture 
//////////////////

/// <summary> 
/// 获得原始yuv422格式图像，DS4100、DS4000HC原始图像是4CIF图像格式(包括QCIF编码)， 
/// DS-4000HS原始图像为CIF图像格式 
/// </summary> 
/// <param name="hChannelHandle">通道句柄</param> 
/// <param name="ImageBuf">原始yuv422格式图像指针</param> 
/// <param name="Size">原始yuv422格式图像尺寸，函数调用前是ImageBuf的大小，调用后是实际图像所占用的字节数</param> 
/// <returns>成功返回0，失败返回错误号</returns> 
DVRS_API int __stdcall GetOriginalImage(HANDLE hChannel, UCHAR *ImageBuf, ULONG *Size);

DVRS_API int __stdcall LoadYUVFromBmpFile(TCHAR *FileName, unsigned char *yuv, int BufLen, int *Width, int *Height);

/// <summary> 
/// 用户程序可调用此函数来生成24位的bmp文件，DS4100、DS4000HC卡抓图 
/// Width 为704，Height 为576PAL/480NTSC。 
/// </summary> 
/// <param name="FileName">文件名</param> 
/// <param name="yuv">yuv422格式图像指针</param> 
/// <param name="Width">图像宽度</param> 
/// <param name="Height">图像高度</param> 
/// <returns>成功返回0 ，失败返回错误号</returns> 
DVRS_API int __stdcall SaveYUVToBmpFile(TCHAR *FileName, unsigned char *yuv, int Width, int Height);


////////////////////////////////
//Osd
/////////////////////////////////////
DVRS_API int __stdcall SetOsd(HANDLE hChannel, BOOL Enable);
DVRS_API int __stdcall SetOsdDisplayMode(HANDLE hChannel, int Brightness, BOOL Translucent, int TwinkleInterval, USHORT *Format1, USHORT *Format2);
DVRS_API int __stdcall SetLogoDisplayMode(HANDLE hChannel, COLORREF ColorKey, BOOL Translucent, int TwinkleInterval);
DVRS_API int __stdcall SetLogo(HANDLE hChannel, int x, int y, int w, int h, unsigned char *yuv);
DVRS_API int __stdcall StopLogo(HANDLE hChannel);

///////////////////////////////////////////////////////////////////////
//motion
/////////////////////////////////////////////////////////////////////
DVRS_API int __stdcall StartMotionDetection(HANDLE hChannel);
DVRS_API int __stdcall AdjustMotionDetectPrecision( HANDLE hChannel, int iGrade, int iFastMotionDetectFps, int iSlowMotionDetectFps);
DVRS_API int __stdcall SetupMotionDetection(HANDLE hChannel, RECT *RectList, int iAreas);
DVRS_API int __stdcall MotionAnalyzer(HANDLE hChannel, char *MotionData, int iThreshold, int *iResult);
DVRS_API int __stdcall StopMotionDetection(HANDLE hChannel);

DVRS_API int __stdcall SetVideoFrameRate(HANDLE hChannel, int nFrameRate);

//////////////////////////////
//Audio
//////////////////////////////
DVRS_API int __stdcall SetAudioPreview(HANDLE hChannel, BOOL bEnable);
DVRS_API int __stdcall  UpdateVideoPreview();


DVRS_API int __stdcall SetDefaultDisplayPicture(HANDLE hChannel, TCHAR *pFileName);
DVRS_API int __stdcall SetupMask(HANDLE hChannel, RECT *rectList, int iAreas);
DVRS_API int __stdcall StopMask(HANDLE hChannel);

DVRS_API int __stdcall SetBitrateControlMode(HANDLE hChannel, BitrateControlType_t brc);
DVRS_API int __stdcall SetIBPMode(HANDLE hChannel, int KeyFrameIntervals, int BFrames, int PFrames, int FrameRate);
DVRS_API int __stdcall SetupBitrateControl(HANDLE hChannel, ULONG MaxBps);
DVRS_API int __stdcall SetDefaultQuant(HANDLE hChannel, int IQuantVal, int PQuantVal, int BQuantVal);
DVRS_API int __stdcall SetVideoSharp(HANDLE hChannel, int nLevel);//8 Level: 0~7
DVRS_API int __stdcall SetAudioVolume(HANDLE hChannel, int nVolume);  //range: 0(min)~100(max)
DVRS_API int __stdcall AdjustForLargeChannles();
DVRS_API int __stdcall SetPreviewOsdMode(BOOL bOsdMode);
DVRS_API int __stdcall GetBoardInfoEx(HANDLE hChannel, ULONG *BoardType, UCHAR *buffer );
DVRS_API int __stdcall GetSystemBoardInfo(SYSTEM_BOARD_INFO* pBoardInfo);
DVRS_API int __stdcall SetDefaultDisplayLanguage(HANDLE hChannel, int nLanguage);
DVRS_API int __stdcall LowLevelVideoPreview(int offset=1);
DVRS_API int __stdcall UpdateVideoPreviewEx(int nCount, int index=0);
DVRS_API int __stdcall GetVideoPreviewModeInfo(int nCount, stPreviewMode* pModeInfo);
DVRS_API int __stdcall AddVideoPreviewModeInfo(stPreviewMode* pModeInfo);
DVRS_API int __stdcall SetVideoPreviewIndex(HANDLE hChannel, int nIndex);
DVRS_API int __stdcall RegisterDrawFun(DWORD nport, DRAWFUN_CALLBACK drawfun, LONG nUser);
DVRS_API int __stdcall ConfigVideoEncode( int nMode, int nIntensity );
DVRS_API int __stdcall ConfigAutoAdjust( int nEnable, int nMinCpu, int nMaxCpu );
DVRS_API int __stdcall ConfigDvrLevel( int nLevel );
DVRS_API int __stdcall ForceVideoFormat( int nForce );

DVRS_API int __stdcall GetCurrentVideoFormat(HANDLE hChannel, PictureFormat_t* pPictureFormat, VideoStandard_t* pVideoStandard=NULL, int* pWidth=NULL, int* pHight=NULL );
DVRS_API int __stdcall CryptoDogCheck(HANDLE hChannel, BYTE* buffer);
DVRS_API int __stdcall CryptoDogConfig(HANDLE hChannel, DWORD dwReadPassword, DWORD dwWritePassword, BYTE* Ci, BYTE* Sk, BYTE* Gc);
DVRS_API int __stdcall CryptoDogAccess(HANDLE hChannel, BOOL bWrite, int nAddress, int nNumber, BYTE* buffer);
DVRS_API int __stdcall CryptoDogCheckEx(HANDLE hChannel, BYTE* buffer, DWORD dwID, BYTE* Data, DWORD* pdwLength );
DVRS_API int __stdcall SetImageStream(HANDLE hChannel, BOOL bStart, UINT fps, UINT width, UINT height, unsigned char**imageBuffer, bool bVideoIn=false);
DVRS_API int __stdcall StartVideoOutput(int iBoardNo, int chIndex);
DVRS_API int __stdcall GpioInit(int iBoardNo, DWORD dwData);
DVRS_API int __stdcall GpioSetData(int iBoardNo, DWORD dwData);
DVRS_API int __stdcall GpioGetData(int iBoardNo, DWORD* pdwData);


DVRS_API int __stdcall SetVideoChangeProperty(HANDLE hChannel);
DVRS_API int __stdcall ReadReg(HANDLE hChannel, PDRV_RW pRW);
DVRS_API int __stdcall WriteReg(HANDLE hChannel, DRV_RW rw);
DVRS_API int __stdcall RegisterDrawFun(DWORD nport, DRAWFUN_CALLBACK DrawFun, LONG nUser);

DVRS_API int __stdcall GetBoardData(HANDLE hChannel, BYTE* buffer, int nSize);
DVRS_API int __stdcall GetBoardDataEx(HANDLE hChannel, BYTE* buffer, int nSize);
DVRS_API int __stdcall SetOsdColor(HANDLE hChannel, COLORREF timecolor, COLORREF titlecolor);
DVRS_API int __stdcall SetPreview(HANDLE hChannel, BOOL bPreview);
DVRS_API int __stdcall GBI2CWrite(HANDLE hChannel, DWORD addr, DWORD value);

DVRS_API int __stdcall StopDMA(HANDLE hChannel);
DVRS_API int __stdcall StartDMA(HANDLE hChannel);
DVRS_API int __stdcall ReStartDsp(HANDLE hChannel);

DVRS_API int __stdcall WriteZone(HANDLE hChannel, unsigned char wr_zone, unsigned char wr_addr, unsigned char wr_number, unsigned char *pData);
DVRS_API void* __stdcall ReadZone(HANDLE hChannel, unsigned char rd_zone, DWORD rd_addr, unsigned char rd_number);
DVRS_API int __stdcall GetSysDeviceID(HANDLE hChannel);
DVRS_API int __stdcall ReInit_Video_Derive(HANDLE hChannel);
DVRS_API int __stdcall GetInit_Video_Status(HANDLE hChannel);
DVRS_API int __stdcall SetInit_Video_Status(HANDLE hChannel, int status);
DVRS_API int __stdcall Change_Video_OVSW(HANDLE hChannel, int status);
DVRS_API int __stdcall Get_DSP_TotalChnnel(HANDLE hChannel);
DVRS_API BOOL __stdcall ReOpen_USB_Device(HANDLE hChannel);

DVRS_API int __stdcall RegisterVideoStatusCallback(VIDEO_STATUS_CALLBACK VideoStatusCallback, void *Context);
DVRS_API int __stdcall Set_Main_Enable(HANDLE hChannel, int status);
DVRS_API int __stdcall Set_Sub_Enable(HANDLE hChannel, int status);
DVRS_API int __stdcall Set_Sub_QCIF_Enable(HANDLE hChannel, int status);
DVRS_API int __stdcall SetRecoderFrameRate(HANDLE hChannel, int nFrameRate);
DVRS_API int __stdcall Set_Sys_PowerDown(HANDLE hChannel, BOOL bEn);
DVRS_API int __stdcall GetSystem_Version(HANDLE hChannel);
# ifdef __cplusplus
}
# endif
#endif