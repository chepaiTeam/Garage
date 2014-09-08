////////////////////////////////////////////////////////////////////////////////
//DvrsCap.h
//////////////////////////////////////////////////////////////////////////////

#ifndef _DVRS_CAP_SDK_H_
#define _DVRS_CAP_SDK_H_

#include "datatype.h"

//Ԥ���������з�ʽ����(��Ҫ��Ϊoverlay��ʽ��)
typedef struct _PREVIEWRC
{
	int   sizeUnit;				//�����ڵĴ�С�������Ա�׼��λ�ƣ�
	int   xUnit, yUnit;			//��������λ�ã��Ա�׼��λ�ƣ�
} stPreviewRC, *pstPreviewRC; 
typedef struct _PREVIEWMODE
{
	int nPreviewCount;			//��������
	int hUnit, vUnit;			//x, y�����ϵı�׼��λ��Ŀ
	int nWidth, nHeight;		//��׼Ԥ���Ŀ�͸�(overlay)
	int maxWidth, maxHeight;	//���Ԥ���Ŀ�͸�(offscreen)
	stPreviewRC* nPreviewPos;	//Ԥ�����еĴ�С��λ����Ϣ����ָ��(���ٰ�������������)
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
/// ֱ�Ӷ�ȡ�����ص�����
/// </summary>
/// <param name="channelNumber">ͨ����</param>
/// <param name="context">�豸������</param>
/// <returns></returns>
typedef int (*STREAM_READ_CALLBACK)(ULONG channelNumber, void *context);
typedef int (*VIDEO_STATUS_CALLBACK)(ULONG channelNumber, BOOL bStatus, void *context);

/// <summary>
/// ����������ֱ�Ӷ�ȡ�ص�����
/// </summary>
/// <param name="channelNumber">ͨ����</param>
/// <param name="DataBuf">��������ַ</param>
/// <param name="Length">����������</param>
/// <param name="FrameType">����������֡����</param>
/// <param name="context">�豸������</param>
/// <returns></returns>
typedef int (*STREAM_DIRECT_READ_CALLBACK)(ULONG channelNumber, void *DataBuf, DWORD Length, int FrameType, void *context);


DVRS_API int __stdcall Test(HANDLE hChannel);
/// 1.�忨��ʼ����ж��

/// <summary>
/// 1.1��ʼ��DSP InitDSPs
///     ˵  ���� ��ʼ��ϵͳ��ÿһ��忨��Ӧ��Ӧ�������������ʱ��ɡ��������ֵΪ0���
///     ����ʼ��ʧ�ܣ�����û���ҵ���Ӧ��DSP���ģ�顣
/// 
/// </summary>
/// <returns>ϵͳ�ڿ��õı���ͨ�������� </returns>
DVRS_API int __stdcall InitDSPs();

/// <summary>
/// 1.2ж��DSP DeInitDSPs
///     ˵  ����  �ر�ÿһ��忨�ϵĹ��ܣ�Ӧ��Ӧ����������˳�ʱ���á� 
/// 
/// int __stdcall DeInitDSPs()
/// </summary>
/// <returns>0</returns>
DVRS_API int __stdcall DeInitDSPs();
DVRS_API HANDLE __stdcall ChannelOpen(int nChannel);
DVRS_API int __stdcall ChannelClose(HANDLE hChannel);

/// <summary>
/// 2.3��ȡϵͳ�б���ͨ���ĸ���
///     ˵  ����  ��ȡϵͳ�����б��뿨�ı���ͨ���ܸ���������Hϵ�к�HCϵ�б��뿨��
/// 
/// </summary>
/// <returns>ϵͳ�б���ͨ���ĸ���</returns>
DVRS_API int __stdcall GetTotalChannels();

/// <summary>
/// 2.2��ȡϵͳ��DSP�ĸ���
///     ˵  ����  ��ȡϵͳ�����а忨��DSP�ĸ����� 
/// 
/// </summary>
/// <returns>ϵͳ��DSP���ܸ���</returns>
DVRS_API int __stdcall GetTotalDSPs();

DVRS_API int __stdcall StopAllDSPs();
DVRS_API int __stdcall ReStartAllDSPs();
DVRS_API int __stdcall StartDsp(HANDLE hChannel);
DVRS_API int __stdcall StopDsp(HANDLE hChannel);

//////////////////////////////////////////////
// preview 
////////////////////////////////////////////////////////

/// <summary> 
/// SDK��3.2�汾���ڲ����Կ���ʵ����HC����overlay��ʽԤ���Ĺ��ܣ��˹� 
/// �ܲ�֧����H������״̬�£����������Ԥ���Ļ��ʺͽ���CPU�����ʡ���Ԥ������С 
/// ��704*576ʱ����Ҫ���øú���������overlayģʽ���粻�������Զ��л���offscreenģʽ 
/// ����Ԥ����ʾ����Ԥ���������704*576ʱ��SDK�Զ��л���overlayģʽ 
/// </summary> 
/// <param name="bTrue">�Ƿ�����overlayԤ����ʽ��Ҳ������MD��</param> 
/// <returns>0��ʾ�Կ�֧�ְ忨��overlayԤ����ʽ������ֵ��ʾ�Կ���֧��</returns> 
DVRS_API int __stdcall SetPreviewOverlayMode(BOOL bTrue);

/// <summary> 
/// ������ƵԤ��������SetPreviewOverlayMode�󣬿ɽ���overlayģʽԤ���� 
/// ���򣬽�Ĭ�ϲ���offscreenģʽԤ�������������704*576ʱ��SDK�Զ��л���overlayԤ��ģʽ�� 
/// </summary> 
/// <param name="hChannelHandle"></param> 
/// <param name="WndHandle"></param> 
/// <param name="rect"></param> 
/// <param name="bOverlay"></param> 
/// <param name="VideoFormat"></param> 
/// <param name="FrameRate"></param> 
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns> 
DVRS_API int __stdcall StartVideoPreview(HANDLE hChannel, HWND WndHandle, RECT *rect, BOOL bOverlay, int VideoFormat, int FrameRate);

/// <summary> 
/// ֹͣ��ƵԤ�� 
/// </summary> 
/// <param name="hChannelHandle">ͨ�����</param> 
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns> 
DVRS_API int __stdcall StopVideoPreview(HANDLE hChannel);

/// <summary> 
/// �忨����ʾ��ΧС��704*576ʱ������SetPreviewOverlayMode���Կ���overlay 
/// Ԥ��ģʽ�������SetOverlayColorKey����overlay�ؼ�ɫ������ʾ��Χ����704*576ʱ�� 
/// �忨�Զ��л���overlayԤ��ģʽ���ؼ�ɫĬ������ΪRGB��10��10��10����Ҳ�ɵ��� 
/// SetOverlayColorKey�޸Ĺؼ�ɫ��������������£�����Ҫ����ʾ���ڵĵ�ɫ����Ϊ�͹ؼ� 
/// ɫ��һ�¡�����ͼ��������ʾ�� 
/// </summary> 
/// <param name="DestColorKey">overlay�ؼ�ɫ������RGB��*��*��*����</param> 
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns> 
DVRS_API int __stdcall SetOverlayColorKey(COLORREF DestColorKey);


/// <summary> 
/// ������Ƶ���� 
/// </summary> 
/// <param name="hChannelHandle">ͨ�����</param> 
/// <param name="Brightness">����ֵ��0-255��</param> 
/// <param name="Contrast">�Աȶȣ�0-127��</param> 
/// <param name="Saturation">���Ͷȣ�0-127��</param> 
/// <param name="Hue">ɫ����0-255��</param> 
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns> 
DVRS_API int __stdcall SetVideoPara(HANDLE hChannel, int Brightness, int Contrast, int Saturation, int Hue);

/// <summary> 
/// ��ȡ��Ƶ���� 
/// </summary> 
/// <param name="hChannelHandle">ͨ�����</param> 
/// <param name="VideoStandard">��Ƶ��ʽ 
/// StandardNone������Ƶ�ź� 0x80000000 
/// StandardNTSC��NTSC��ʽ 0x00000001 
/// StandardPAL��PAL��ʽ 0x00000002</param> 
/// <param name="Brightness">����ָ��ֵ��0-255��</param> 
/// <param name="Contrast">�Աȶ�ָ��ֵ��0-127��</param> 
/// <param name="Saturation">���Ͷ�ָ��ֵ��0-127��</param> 
/// <param name="Hue">ɫ��ָ��ֵ��0-255��</param> 
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns> 
DVRS_API int __stdcall GetVideoPara(HANDLE hChannel, VideoStandard_t *VideoStandard, int *Brightness, int *Contrast, int *Saturation, int *Hue);

/// <summary>
/// 3.3.4��ȡ��Ƶ�ź��������GetVideoSignal
///     ˵  ����  ��ȡ��Ƶ�źŵ����������������Ƶ��ʧ���� 
/// 
/// int __stdcall GetVideoSignal(HANDLE  hChannelHandle)
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <returns>�ź���������0����������ֵ˵���ź��쳣���д���</returns>
DVRS_API int __stdcall GetVideoSignal(HANDLE  hChannel);

/// <summary>
/// 3.4.4.3.1������ͨ���ֱ���SetEncodERPictureFormat
///     ˵ ����  ������ͨ������ֱ��ʡ�֧�ֶ�̬�޸ġ�
/// 
/// int __stdcall SetEncoderPictureFormat(HANDLE hChannelHandle,  PictureFormat_t PictureFormat) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="PictureFormat">����ͼ��ֱ��ʣ�4CIF��DCIF��2CIF��CIF��QCIF�� </param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall SetEncoderPictureFormat(HANDLE hChannel, PictureFormat_t PictureFormat);

/// <summary>
/// 3.3.1������Ƶ��ʽSetVideoStandard���˺���ֻ��H����Ч
///     ˵  ����  ������Ƶ��ʽ����ĳһ��ʽ������ͷ�Ѿ��Ӻõ����������ϵͳʱ�ɲ��ص��ø�
///     ���������û�н�����ͷ�����������ϵͳȻ���ٽ�NTSC��ʽ������ͷ�������øú�    
///     ����������;������ͬ��ʽ������ͷҲ������øú�����
/// 
/// int __stdcall SetVideoStandard(HANDLE hChannelHandle,  VideoStandard_t VideoStandard) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="VideoStandard">��Ƶ��ʽ</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall SetVideoStandard(HANDLE hChannel, VideoStandard_t VideoStandard);
DVRS_API int __stdcall StartVideoCapture(HANDLE hChannel);
DVRS_API int __stdcall StopVideoCapture(HANDLE hChannel);
DVRS_API int __stdcall ConfigSubChannelSplit( int nMaxSubCh, int nMode=0 );

/// <summary>
/// 3.4.3.1������ͨ������������SetStreamType
///     ˵  ����  ������ͨ�����������͡��˺���������������ǰ��������
/// 
/// int __stdcall SetStreamType(HANDLE hChannelHandle, USHORT Type) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="Type">������</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall SetStreamType(HANDLE hChannel, USHORT Type);

/// 3.4.3.2��ȡ��ͨ������������GetStreamType
///     ˵  ����  ��ȡ��ͨ������������
/// 
/// int __stdcall GetStreamType(HANDLE hChannelHandle, USHORT *StreamType) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="StreamType">������</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش���� </returns>
DVRS_API int __stdcall GetStreamType(HANDLE hChannel, USHORT *StreamType);

/// <summary>
/// 2.8��ȡ�忨�ͺż����к���ϢGetBoardInfo
///     ˵  ����  ��ȡ�忨���ͺż����к���Ϣ
/// 
/// int __stdcall GetBoardInfo(HANDLE hChannelHandle, ULONG *BoardType,  UCHAR *SerialNo)
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="BoardType">�忨�ͺ�</param>
/// <param name="SerialNo">
/// �忨ID��, ����Ϊ�忨���кŵ�ASCII����ֵ������ΪSerialNo[0] ��Ӧ���λ��
/// SerialNo[11]��Ӧ���λ�����翨��Ϊ��40000002345����ֵ��ӦΪ 4,0,0,0,0,1,0,0,2,3,4,5 ���������顣
/// </param>
/// <returns>�ɹ�Ϊ0��ʧ�ܷ��ش���� </returns>
DVRS_API int __stdcall GetBoardInfo(HANDLE hChannel, ULONG *BoardType, UCHAR *SerialNo);

DVRS_API int __stdcall RegisterLogRecordCallback(LOGRECORD_CALLBACK LogRecordFunc, void *Context);

/// <summary>
/// 3.5.3.2��ȡ��������
///     ˵ ����  ��ָ�����ȵ��������������ڷ�ʽ������ʽ����������StartVideoCapture ��
///     StartMotionDetection��SDK�̻߳�����ע����û�������Ϣ����������ָ������Ϣ��
///     ���ṩ��Ϣ��Դ��ͨ���š����û������յ�����Ϣʱ���ɵ��ñ���������ȡ���ݣ�Length ��
///     ��Ϊ����ʱ�����ṩ����Ĵ�С��ReadStreamData���жϻ����Ƿ��㹻����������㹻��
///     �򷵻�ֵΪ��ǰ�Ķ�ȡ��֡���ȣ����򷵻ش���
///         ��HC���У�����Ѿ��ȵ�����RegisterStreamDirectReadCallback()������������� 
///     ReadStreamData����ȡ���ݣ���Ϊ����Ƶ���ݻ���RegisterStreamDirectReadCallback��ע��
///     �Ļص�������ֱ�ӷ��ء�
/// 
/// int __stdcall ReadStreamData(HANDLE hChannelHandle, void *DataBuf,  DWORD *Length, int *FrameType) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="DataBuf">�Զ�������ݻ�����</param>
/// <param name="Length">���룺�������Ĵ�С�������һ֡���ݵĴ�С</param>
/// <param name="FrameType">֡����</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall ReadStreamData(HANDLE hChannel, void *DataBuf, DWORD *Length, int *FrameType);

/// <summary>
/// 3.5.3.1.2.2ע����Ϣ��ȡ��������
///     ˵ ����  ������׼����ʱ��SDK����hWnd���ڷ���MessageId��Ϣ��Ŀ�괰���յ�
///     Message�����ReadStreamData��ȡһ֡���ݡ����HC����H����壬�����ȵ��� 
///     RegisterStreamDirectReadCallback������ע��HC��ȡ�����ص��������ٵ���
///     RegisterMessageNotifyHandle������ע��H��ȡ������Ϣ������
///     HCϵ�а忨����ʹ�÷�ʽһ�������ݲ��� 
/// 
/// int __stdcall RegisterMessageNotifyHandle(HWND hWnd, UINT MessageId)
/// </summary>
/// <param name="hWnd">ͨ�����</param>
/// <param name="MessageId">�Զ�����Ϣ</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش���� </returns>
DVRS_API int __stdcall RegisterMessageNotifyHandle(HWND hWnd, UINT MessageId);

/// <summary>
/// 3.5.3.1.3.1ע��ֱ�Ӷ�ȡ�����ص�����
///     ˵ ����  ��һ����������ȡ��ʽ
/// 
/// int __stdcall RegisterStreamReadCallback (STREAM_READ_CALLBACK StreamReadCallback, void *Context) 
/// </summary>
/// <param name="StreamReadCallback">ֱ�Ӷ�ȡ�����ص�����</param>
/// <param name="Context">�豸������</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall RegisterStreamReadCallback(STREAM_READ_CALLBACK StreamReadCallback, void *Context);

/// <summary>
/// 3.5.3.2��ȡ��������
///     ˵ ����  ��ָ�����ȵ��������������ڷ�ʽ������ʽ����������StartVideoCapture ��
///     StartMotionDetection��SDK�̻߳�����ע����û�������Ϣ����������ָ������Ϣ��
///     ���ṩ��Ϣ��Դ��ͨ���š����û������յ�����Ϣʱ���ɵ��ñ���������ȡ���ݣ�Length ��
///     ��Ϊ����ʱ�����ṩ����Ĵ�С��ReadStreamData���жϻ����Ƿ��㹻����������㹻��
///     �򷵻�ֵΪ��ǰ�Ķ�ȡ��֡���ȣ����򷵻ش���
///         ��HC���У�����Ѿ��ȵ�����RegisterStreamDirectReadCallback()������������� 
///     ReadStreamData����ȡ���ݣ���Ϊ����Ƶ���ݻ���RegisterStreamDirectReadCallback��ע��
///     �Ļص�������ֱ�ӷ��ء�
/// 
/// int __stdcall ReadStreamData(HANDLE hChannelHandle, void *DataBuf,  DWORD *Length, int *FrameType) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="DataBuf">�Զ�������ݻ�����</param>
/// <param name="Length">���룺�������Ĵ�С�������һ֡���ݵĴ�С</param>
/// <param name="FrameType">֡����</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall RegisterStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);
DVRS_API int __stdcall RegisterSubStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);
DVRS_API int __stdcall RegisterSubQCIFStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);

/// <summary>
/// 3.4.5ǿ���趨I֡CaptureIFrame
///     ˵ ����  ����ǰ����֡ǿ���趨ΪI֡ģʽ���ɴ���������ȡ��֡�����������紫�͡�
/// 
/// int __stdcall CaptureIFrame(HANDLE hChannelHandle)
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall CaptureIFrame(HANDLE hChannel);

/// <summary>
/// 3.4.4.3.1������ͨ���ֱ���SetEncodERPictureFormat
///     ˵ ����  ������ͨ������ֱ��ʡ�֧�ֶ�̬�޸ġ�
/// 
/// int __stdcall SetEncoderPictureFormat(HANDLE hChannelHandle,  PictureFormat_t PictureFormat) 
/// </summary>
/// <param name="hChannelHandle">ͨ�����</param>
/// <param name="PictureFormat">����ͼ��ֱ��ʣ�4CIF��DCIF��2CIF��CIF��QCIF�� </param>
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns>
DVRS_API int __stdcall SetEncoderPictureFormat(HANDLE hChannel, PictureFormat_t PictureFormat);

///////////////////////////
//  capture 
//////////////////

/// <summary> 
/// ���ԭʼyuv422��ʽͼ��DS4100��DS4000HCԭʼͼ����4CIFͼ���ʽ(����QCIF����)�� 
/// DS-4000HSԭʼͼ��ΪCIFͼ���ʽ 
/// </summary> 
/// <param name="hChannelHandle">ͨ�����</param> 
/// <param name="ImageBuf">ԭʼyuv422��ʽͼ��ָ��</param> 
/// <param name="Size">ԭʼyuv422��ʽͼ��ߴ磬��������ǰ��ImageBuf�Ĵ�С�����ú���ʵ��ͼ����ռ�õ��ֽ���</param> 
/// <returns>�ɹ�����0��ʧ�ܷ��ش����</returns> 
DVRS_API int __stdcall GetOriginalImage(HANDLE hChannel, UCHAR *ImageBuf, ULONG *Size);

DVRS_API int __stdcall LoadYUVFromBmpFile(TCHAR *FileName, unsigned char *yuv, int BufLen, int *Width, int *Height);

/// <summary> 
/// �û�����ɵ��ô˺���������24λ��bmp�ļ���DS4100��DS4000HC��ץͼ 
/// Width Ϊ704��Height Ϊ576PAL/480NTSC�� 
/// </summary> 
/// <param name="FileName">�ļ���</param> 
/// <param name="yuv">yuv422��ʽͼ��ָ��</param> 
/// <param name="Width">ͼ����</param> 
/// <param name="Height">ͼ��߶�</param> 
/// <returns>�ɹ�����0 ��ʧ�ܷ��ش����</returns> 
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