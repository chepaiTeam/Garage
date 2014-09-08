
#if ! defined __DATA_TYPE_H_
#define __DATA_TYPE_H_

#define VERSION_01       0x31
#define VERSION_02       0x32
#define VERSION_CODE_XVID      0x01
#define VERSION_CODE_ADPCM     0x02
#define SYSTEM_HEAD_ID     0x00000189
#pragma pack(push,1)
typedef union tag {
         struct {
            unsigned int   witdh         : 16;
            unsigned int   height        : 16;
         } VIDEO;
         struct {
            unsigned int   channel      : 8;
			unsigned int   bit          : 8;
            unsigned int   sample       : 16;
         } AUDIO;
		 struct {
		   unsigned int   version             : 16;
		   unsigned int   codeversion         : 16;
		 }VERSION;
}UNION_INFO;
#pragma pack(pop)
#pragma pack(push,1)
typedef struct tagTFRAME_HEADER
{
	ULONG	IncId;			        /* 0x000000819 */
	USHORT	Length;			        /* lenth include this header */
	BYTE	Magic;                  /*  LOBYTE( length )*/
	BYTE	FrameType;		        /* I frames, P frames or B frames Audio frames or other etc */
	ULONG	FrameNumber;	        /* serial number of this frame */
	ULONG	SysColock;		    	/* system clock when this frames is processed */
	UNION_INFO info1;               /* video  :width  height   audio channel */
	UNION_INFO info2;               /* info 2 */
} TFRAME_HEADER, *PTFRAME_HEADER;
#pragma pack(pop)

/// <summary>
/// 视频制式
/// </summary>
typedef enum 
{
   StandardNone				= 0x80000000,	// 无视频信号
   StandardNTSC				= 0x00000001,	// NTSC制式
   StandardPAL				= 0x00000002,	// PAL制式
   StandardSECAM			= 0x00000004,
} VideoStandard_t;

/// <summary>
/// 编码图像分辨率
/// </summary>
typedef enum
{
	ENC_CIF_FORMAT		= 0, // 352*288 or 352*240
	ENC_QCIF_FORMAT		= 1, // 176*144 or 176*120
	ENC_2CIF_FORMAT		= 2, // 704*288 or 704*240
	ENC_4CIF_FORMAT		= 3, // 704*576 or 704*480
	ENC_SCIF_FORMAT		= 4, // 240*192 or 240*164
	ENC_SVGA_FORMAT		= 5, // 640*288 or 640*240
	ENC_VGA_FORMAT		= 6, // 640*480
	ENC_DCIF_FORMAT		= 7  // 320*240
} PictureFormat_t;


/// <summary>
/// 视频预览格式
/// </summary>
typedef enum
{
   vdfRGB8A_233              = 0x00000001,
   vdfRGB8R_332              = 0x00000002,
   vdfRGB15Alpha             = 0x00000004,
   vdfRGB16                  = 0x00000008,
   vdfRGB24                  = 0x00000010,
   vdfRGB24Alpha             = 0x00000020,
   
   vdfYUV420Planar           = 0x00000040,
   vdfYUV422Planar           = 0x00000080,
   vdfYUV411Planar           = 0x00000100,
   vdfYUV420Interspersed     = 0x00000200,
   vdfYUV422Interspersed     = 0x00000400,
   vdfYUV411Interspersed     = 0x00000800,
   vdfYUV422Sequence         = 0x00001000,   /* U0, Y0, V0, Y1:  For VO overlay */
   vdfYUV422SequenceAlpha    = 0x00002000,   /* U0, Y0, V0, Y1:  For VO overlay, with low bit for alpha blending */
   vdfMono                   = 0x00004000,   /* 8 bit monochrome */

   vdfYUV444Planar           = 0x00008000,
} TypeVideoFormat;

/// <summary>
/// 音频格式
/// </summary>
typedef enum
{
	adf_8MONO		= 0,
	adf_8STEREO		= 1,
	adf_16MONO		= 2,
	adf_16STEREO	        = 3,
} TypeAudioFormat;


typedef enum
{
	PktError				= 0,
	PktIFrames				= 1,
	PktBBPFrames			= 2,
	PktPFrames				= 3,
	PktAudioFrames			= 4,
	PktMotionDetection		= 5,
	PktDspStatus			= 6,
	PktOrigImage			= 7,
	PktSysHeader			= 8,
	PktBPFrames				= 9,
	PktSFrames				= 10,
	PktSubIFrames			= 11,
	PktSubPFrames			= 12,
	PktSubBBPFrames			= 13,
	PktSubSysHeader			= 14
} FrameType_t;


/// <summary>
/// 码流控制方式
/// </summary>
typedef enum
{
	brCBR = 0,		// 变码率
	brVBR = 1,		// 恒定码率
	brHYBRID = 2,
} BitrateControlType_t;

typedef struct tagDRV_RW
{
   DWORD dwType;
   DWORD dwAddr;
   DWORD pData;
}DRV_RW,*PDRV_RW;


typedef struct tagVIDEO_INFO
{
   int nSubChannel;
   VideoStandard_t video_stand;
   RECT rect;
}VIDEO_INFO,*PVIDEO_INFO;

#define _OSD_BASE				0x9000
#define	_OSD_YEAR4				_OSD_BASE+0		/* user defined ASCII OSD */
#define _OSD_YEAR2				_OSD_BASE+1		/* 2 chars year display，such as 02*/
#define _OSD_MONTH3				_OSD_BASE+2		/* month display，such as Jan */
#define _OSD_MONTH2				_OSD_BASE+3		/* month display，such as 01 */
#define _OSD_DAY				_OSD_BASE+4		/* day display, such as 31 */
#define _OSD_WEEK3				_OSD_BASE+5		/* week display, such as Tue */
#define	_OSD_CWEEK1				_OSD_BASE+6		/* week number display, such as No.37 week(18,Sep.) */
#define	_OSD_HOUR24				_OSD_BASE+7		/* hour display 24, such as 18 */
#define	_OSD_HOUR12				_OSD_BASE+8		/* hour display 12, such as AM09, PM09 */
#define	_OSD_MINUTE				_OSD_BASE+9		/* 2 chars minutes display */
#define _OSD_SECOND				_OSD_BASE+10	/* 2 chars seconds display */
#define _OSD_USERTYPE_ASCII		_OSD_BASE+11	/* user defined ASCII char OSD */
#define	_OSD_USERTYPE_CHINESE	_OSD_BASE+12	/* user defined CHINESE char OSD */


#define DISPLAY_LANG_MIN	   				1
#define DISPLAY_LANG_ENGLISH				1    /* English default display */
#define DISPLAY_LANG_CHINESE_SIMPLIFIED		2    /* Chinese simplified */
#define DISPLAY_LANG_CHINESE_TRADITIONAL	3    /* Chinese traditional */
#define DISPLAY_LANG_JAPANESE				4    /* Japanese default display */
#define DISPLAY_LANG_KOREAN 				5    /* Korean default display */
#define DISPLAY_LANG_FRENCH 				6    /* French default display */
#define DISPLAY_LANG_GERMAN 				7    /* German default display */
#define DISPLAY_LANG_RUSSIAN 				8    /* Russian default display */
#define DISPLAY_LANG_ITALIAN 				9    /* Italian default display */
#define DISPLAY_LANG_THAI   				10   /* Thai default display */
#define DISPLAY_LANG_MALAY   				11   /* Malay default display */
#define DISPLAY_LANG_ARABIC   				12   /* Arabic default display */
#define DISPLAY_LANG_MAX	   				DISPLAY_LANG_ARABIC



#define MAX_DSP_COUNT		8
#define MAX_BOARD_COUNT		8
typedef struct _gvBoardInfo
{
	short		vendorID;						//板卡的制造商标识码
	short		deviceID;						//板卡的设备类型标识码
	ULONG		dspStartNumber;					//第一个物理DSP的起始通道号
	ULONG		physicalDspCount;				//板卡上的物理DSP总数
	ULONG		dspChannelCount;				//每个物理DSP的通道数
	ULONG		totalChannelCount;				//板卡上的通道总数
    ULONG       snLength[MAX_DSP_COUNT];		//物理DSP上序列号长度
    UCHAR       snData[MAX_DSP_COUNT][40];		//物理DSP上的序列号数据
} BOARD_INFO, *PBOARD_INFO;

typedef struct tagSystemBoardInfo
{
	ULONG			totalBoardCount;			//系统中的板卡总数
	ULONG			totalphysicalDsp;			//系统中的物理DSP总数
	BOARD_INFO	board[MAX_BOARD_COUNT];			//板卡信息
} SYSTEM_BOARD_INFO, *PSYSTEM_BOARD_INFO;

#define STREAM_TYPE_VIDEO					1
#define STREAM_TYPE_AUDIO					2
#define STREAM_TYPE_AVSYNC					3
#endif