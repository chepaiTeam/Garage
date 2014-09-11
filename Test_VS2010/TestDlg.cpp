// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
//#include "JpegFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_VIDEOSTATIC 9001

typedef struct
{
	int i;
	int j;
	BYTE *TempBuffer;
	CTestDlg *ui;	//
} CV_INFO, *pCV_INFO;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
int g_TotalDSPs = 0;
int g_TotalChannel = 0;
HANDLE g_hDSP[MAX_VIDEO_CHANNEL];
int StreamReadCallback(ULONG i, void *Context)
{
	CTestDlg *dlg = (CTestDlg *)Context;
	if(dlg)
	{
		dlg->SendAndSaveData(i);
	}
	return 0;
}
int StreamDirectReadCallback(ULONG i, void *DataBuf, DWORD Length, int FrameType, void *context)
{
	CTestDlg *dlg = (CTestDlg *)context;
	if(dlg)
	{
		dlg->StreamDirectRead(i, DataBuf, Length, FrameType);
	}

	return 0;
}

// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for(int i = 0; i<MAX_VIDEO_CHANNEL; i++)
	{
		g_hDSP[i] = NULL;
		m_pAviFile[i] = NULL;
		m_bCanWriteFile[i] = false;
		m_bRecord[i] = false;
		m_framerate[i] = 0;
		m_pMediaFile[i] = new CMediaFile();
	}
	m_nFrameRate = 0;
	lBitRateValue =0;
	m_nSubChannel =1;
	m_bCapture = false;
	m_nCount = 0xf;

	m_pCamInfos = NULL;
	for (int i = 0; i < m_LedInfo.TextRgbs.GetCount(); i++)
	{
		delete m_LedInfo.TextRgbs[i];
	}
	m_LedInfo.TextRgbs.RemoveAll();

	for (int i = 0; i < m_LedInfo.TagRgbs.GetCount(); i++)
	{
		delete m_LedInfo.TagRgbs[i];
	}
	m_LedInfo.TagRgbs.RemoveAll();

	m_bDataRun = true;
}

CTestDlg::~CTestDlg()
{
	if (m_pCamInfos)
	{
		delete m_pCamInfos;
		m_pCamInfos = NULL;
	}

	int i;
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopVideoCapture(g_hDSP[i]);
			StopSaveFile(i);

		}
	}
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopDsp(g_hDSP[i]);
		}
	}
	Sleep(1);
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			ChannelClose(g_hDSP[i]);
		}
	}

	DeInitDSPs();
	g_TotalChannel = 0;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	DDX_Control(pDX, IDC_COMBO3, m_Type);

	DDX_Control(pDX, IDC_SLIDER_SH, m_Slider_SH);
	DDX_Control(pDX, IDC_VOLUME, m_Slider_Volume);
	DDX_Control(pDX, IDC_SLIDER_REAT, m_Slider_FrameRate);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider_BR);
	DDX_Control(pDX, IDC_SLIDER2, m_Slider_CO);
	DDX_Control(pDX, IDC_SLIDER3, m_Slider_SA);
	DDX_Control(pDX, IDC_SLIDER4, m_Slider_HU);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STC_XD1, m_stcXD[0]);
	DDX_Control(pDX, IDC_STC_XD2, m_stcXD[1]);
	DDX_Control(pDX, IDC_STC_XD3, m_stcXD[2]);
	DDX_Control(pDX, IDC_STC_XD4, m_stcXD[3]);
	DDX_Control(pDX, IDC_CBOXD, m_cboXD);
	DDX_Control(pDX, IDC_EDIT3, m_edtXH[0]);
	DDX_Control(pDX, IDC_EDIT4, m_edtXH[1]);
	DDX_Control(pDX, IDC_EDIT6, m_edtXH[2]);
	DDX_Control(pDX, IDC_EDIT5, m_edtXH[3]);
	DDX_Control(pDX, IDC_BUTTON36, m_btnBC);
	DDX_Control(pDX, IDC_BUTTON37, m_btnYL);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_REAT, OnReleasedcaptureSliderReat)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonInit)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON25, OnButton25)
	ON_BN_CLICKED(IDC_BUTTON26, OnButton26)
	ON_BN_CLICKED(IDC_BUTTON27, OnButton27)
	ON_BN_CLICKED(IDC_BUTTON28, OnButton28)
	ON_BN_CLICKED(IDC_BUTTON29, OnButton29)
	ON_BN_CLICKED(IDC_BUTTON30, OnButton30)
	ON_BN_CLICKED(IDC_BUTTON31, OnButton31)
	ON_BN_CLICKED(IDC_BUTTON32, OnButton32)
	ON_BN_CLICKED(IDC_BUTTON33, OnButton33)
	ON_BN_CLICKED(IDC_BUTTON34, OnButton34)
	ON_BN_CLICKED(IDC_BUTTON35, OnButton35)
	ON_BN_CLICKED(IDC_BUTTON38, OnButton38)
	ON_BN_CLICKED(IDC_BUTTON39, OnButton39)
	ON_BN_CLICKED(IDC_BUTTON40, OnButton40)
	ON_BN_CLICKED(IDC_BUTTON41, OnButton41)
	ON_BN_CLICKED(IDC_BUTTON42, OnButton42)
	ON_BN_CLICKED(IDC_BUTTON43, OnButton43)
	ON_BN_CLICKED(IDC_BUTTON44, OnButton44)
	ON_BN_CLICKED(IDC_BUTTON45, OnButton45)
	ON_BN_CLICKED(IDC_BUTTON46, OnButton46)
	ON_BN_CLICKED(IDC_BUTTON47, OnButton47)

	ON_BN_CLICKED(IDC_BUTTON2X2, OnButton2x2)
	ON_BN_CLICKED(IDC_BUTTON3X3, OnButton3x3)
	ON_BN_CLICKED(IDC_BUTTON4X4, OnButton4x4)
	ON_BN_CLICKED(IDC_BUTTON6X6, OnButton6x6)

	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	ON_CBN_CLOSEUP(IDC_COMBO2, OnCloseupCombo2)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_VIEW_CLICK, OnMsgViewClick)
	ON_MESSAGE(WM_VIEW_DBCLICK, OnMsgViewDbClick)
	ON_BN_CLICKED(IDC_BUTTON36, &CTestDlg::OnBnClickedButton36)
	ON_BN_CLICKED(IDC_BUTTON37, &CTestDlg::OnBnClickedButton37)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog


	SetIcon(m_hIcon, TRUE);// Set big icon
	SetIcon(m_hIcon, FALSE);// Set small icon

	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);



	RegisterMessageNotifyHandle(this->GetSafeHwnd(), 1900);
	TCHAR buf[MAX_PATH+1];
	DWORD res = GetModuleFileName(AfxGetInstanceHandle(), buf, MAX_PATH);
	strDir = GetBaseDir(buf);

	// TODO: Add extra initialization here
	m_Type.AddString(_T("byte"));
	m_Type.AddString(_T("word"));
	m_Type.AddString(_T("dword"));
	m_Type.SetCurSel(0);

	m_Combo1.AddString(_T("PAL"));
	m_Combo1.AddString(_T("NTSC"));
	m_Combo1.SetCurSel(0);

	m_Combo2.AddString(_T("QCIF"));
	m_Combo2.AddString(_T("DCIF"));
	m_Combo2.AddString(_T("CIF"));
	m_Combo2.AddString(_T("SVGA"));
	m_Combo2.AddString(_T("2CIF"));
	m_Combo2.SetCurSel(2);

	for (int i=0; i<36; i++)
	{
		m_pVideoView[i]  = new CVideoView();

		m_pVideoView[i]->Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + i);
	}

	InitRect();

	OnButton4x4();

	m_nSubChannel =7;
	ConfigSubChannelSplit(7, 2);
	m_nindex = 0;

	m_Slider_FrameRate.SetRange( 0, 25, TRUE);
	m_Slider_FrameRate.SetTicFreq(1);
	m_Slider_FrameRate.SetPageSize(1);
	m_Slider_FrameRate.SetPos(15);



	int nPos = m_Slider_FrameRate.GetPos();
	SetDlgItemInt(IDC_STATIC_FRAMERATE, nPos);


	m_Slider_BR.SetRange( 0, 225, TRUE);
	m_Slider_BR.SetTicFreq(1);
	m_Slider_BR.SetPageSize(1);
	m_Slider_BR.SetPos(100);
	SetDlgItemInt(IDC_STATIC1, 100);

	m_Slider_CO.SetRange( 0, 210, TRUE);
	m_Slider_CO.SetTicFreq(1);
	m_Slider_CO.SetPageSize(1);
	m_Slider_CO.SetPos(100);
	SetDlgItemInt(IDC_STATIC2, 100);

	m_Slider_SA.SetRange( 0, 250, TRUE);
	m_Slider_SA.SetTicFreq(1);
	m_Slider_SA.SetPageSize(1);
	m_Slider_SA.SetPos(100);
	SetDlgItemInt(IDC_STATIC3, 100);

	m_Slider_HU.SetRange( 0, 255, TRUE);
	m_Slider_HU.SetTicFreq(1);
	m_Slider_HU.SetPageSize(1);
	m_Slider_HU.SetPos(100);
	SetDlgItemInt(IDC_STATIC4, 100);

	m_Slider_Volume.SetRange( 0, 48, TRUE);
	m_Slider_Volume.SetTicFreq(1);
	m_Slider_Volume.SetPageSize(1);
	m_Slider_Volume.SetPos(28);

	m_Slider_SH.SetRange( 0, 7, TRUE);
	m_Slider_SH.SetTicFreq(1);
	m_Slider_SH.SetPageSize(1);
	m_Slider_SH.SetPos(4);
	SetDlgItemInt(IDC_STATIC5, 4);

	int i;
	for( i = 0; i<MAX_VIDEO_CHANNEL; i++)
	{
		g_hDSP[i] = (HANDLE)0xffffffff;
	}
	RegisterStreamReadCallback(StreamReadCallback, this);
	RegisterStreamDirectReadCallback(StreamDirectReadCallback, this);
	SetTimer(TIMER_MAIN, 1000, 0);


	ShowResult();
	SetDlgItemText(IDC_EDIT1, _T("0"));

	//OnButton38();

	EnableCaptureButton();

	m_cboXD.AddString(_T("全部"));
	m_cboXD.AddString(_T("信道1"));
	m_cboXD.AddString(_T("信道2"));
	m_cboXD.AddString(_T("信道3"));
	m_cboXD.AddString(_T("信道4"));
	m_cboXD.SetCurSel(0);

	m_nIDs[0] = -1;
	m_nIDs[1] = -1;
	m_nIDs[2] = -1;
	m_nIDs[3] = -1;

	//启动串口1数据采集
	m_pPort1 = new CSerialPort(); 
	m_pPort2 = new CSerialPort(); 

	if (m_pPort1->Open(_T("COM1"), 9600U, 'N', 8U, 1U, EV_RXCHAR, 1024U)
		&& m_pPort2->Open(_T("COM2"), 9600U, 'N', 8U, 1U, EV_RXCHAR, 1024U))
	{
		m_pPort1->StartMonitoring();
		m_pPort1->WriteToPort(_T("@1A1&"));
		//m_pPort->WriteToPort(_T("@1B000&"));
		
		m_pPort2->StartMonitoring();
		m_pPort2->WriteToPort(_T("@1A1&"));

		hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DataThread, this, 0, &ThreadID);
		hLedThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LedThread, this, 0, &LedThreadID);
	}else{
		::MessageBox(this->GetSafeHwnd(), _T("连接设备失败，请检查相关接口！"), _T("警告"), MB_ICONERROR|MB_OK);
	}

	return TRUE;// return TRUE  unless you set the focus to a control
}

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))


void CTestDlg::LedThread(LPVOID pParam)
{
	CTestDlg *ui = (CTestDlg*)pParam;

	while(ui->m_bDataRun)
	{
		BYTE LedStr[100];
		int nLen = ui->m_pPort1->ReadFormPort(LedStr);
		if (nLen > 0)
		{
			if (LedStr[0] == '/' && (LedStr[3] - 48) == ui->m_LedInfo.uLocation && ui->m_sNullCarStr.GetLength() > 0)
			{
				ui->m_pPort1->WriteToPort(ui->m_sNullCarStr);
			}
		}
		Sleep(10);
	}
}

void CTestDlg::CVData(LPVOID pParam)
{
	CV_INFO *pCvInfo = (CV_INFO*)pParam;
	CTestDlg *ui = pCvInfo->ui;
	BYTE *TempBuffer = pCvInfo->TempBuffer;
	int i = pCvInfo->i;
	int j = pCvInfo->j;

	CString str;

	int nWidth, nHeight, nSize;
	BYTE *source;

	BYTE *buf = NULL;
	BYTE *dest1 = NULL;

	int w, h, Num;
	double R, G, B, y1, u1, v1, y2, u2, v2;

	CvRect ROIRect = {0, 0, 0, 0};

	IplImage* pImg = NULL;
	IplImage* pContourImg = NULL;
	IplImage* pImgEx = NULL;

	//CvvImage cvvImg; //CvvImage类

	unsigned int uCamID = i + j * 4;
	if(ui->m_pCamInfos[uCamID].nEffective == 0)
	{
		return;
	}

	//设置裁剪区域
	ROIRect.x = ui->m_pCamInfos[uCamID].nRectX;
	ROIRect.y = ui->m_pCamInfos[uCamID].nRectY;
	ROIRect.width = ui->m_pCamInfos[uCamID].nRectW;
	ROIRect.height = ui->m_pCamInfos[uCamID].nRectH;

	nWidth = *(short *)TempBuffer;
	nHeight = *(short *)(TempBuffer + 2);
	source = TempBuffer + 4;

	if (nWidth <= 50)
	{
		return;
	}

	nSize = 3 * nWidth * nHeight;

	if (!buf)
	{
		buf = new BYTE [nSize];
	}

	dest1 = buf;

	for (h=0; h<nHeight; h++)
	{

		for (w=0;w<nWidth/2;w++)
		{
			y1 = source[0];
			y2 = source[2];
			v1 = v2 = source[3]-128;
			u1 = u2 = source[1]-128;

			R = (double)(y1 + 1.375 * v1);
			G = (double)(y1 - 0.34375 * u1 - 0.703125 * v1);
			B = (double)(y1 + 1.734375 * u1);
			R = max (0, min (255, R));
			G = max (0, min (255, G));
			B = max (0, min (255, B));

			dest1[0] = (BYTE)B;
			dest1[1] = (BYTE)G;
			dest1[2] = (BYTE)R;
			dest1 += 3;

			R = (double)(y2 + 1.375 * v1);
			G = (double)(y2 - 0.34375 * u1 - 0.703125 * v1);
			B = (double)(y2 + 1.734375 * u1);
			R = max (0, min (255, R));
			G = max (0, min (255, G));
			B = max (0, min (255, B));

			dest1[0] = (BYTE)B;
			dest1[1] = (BYTE)G;
			dest1[2] = (BYTE)R;
			dest1 += 3;

			source += 4;
		}
	}

	if (!pImg)
	{
		pImg = cvCreateImage( cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3 );
		pImgEx = cvCreateImage(cvSize(ROIRect.width, ROIRect.height), IPL_DEPTH_8U, 3);
		pContourImg = cvCreateImage(cvGetSize(pImgEx), IPL_DEPTH_8U, 1);
	}

	memcpy(pImg->imageData, buf, nSize);

	cvSetImageROI(pImg, ROIRect);
	cvCopy(pImg, pImgEx, 0);
	cvResetImageROI(pImg);

	//cvRectangle(pImg, cvPoint( ROIRect.x, ROIRect.y ), cvPoint( ROIRect.x + ROIRect.width, ROIRect.y + ROIRect.height-2 ), cvScalar(200, 0, 0, 0), 1, 8, 0);

	//将原始图像转化为灰阶图像
	cvCvtColor(pImgEx, pContourImg, CV_RGB2GRAY);

	cvCanny(pContourImg ,pContourImg, 50, 100, 3);

	//灰阶图像的平滑与增强处理
	cvSmooth(pContourImg, pContourImg, CV_GAUSSIAN, 5); //高斯滤波

	//灰阶图像转化为二值图像
	cvThreshold(pContourImg, pContourImg, 50, 100, CV_THRESH_BINARY);

	CvMemStorage * storage = cvCreateMemStorage(0);  
	CvSeq * contour = NULL;

	//从二值图像中提取轮廓
	Num = cvFindContours( pContourImg, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);//CV_CHAIN_APPROX_SIMPLE
	//TRACE("The number of Contours is: %d\n", Num);


	//for(; contour!=0; contour=contour->h_next)  
	//{
	//	//获得多边形轮廓
	//	CvSeq *polyContours = cvApproxPoly(contour, sizeof(CvContour), cvCreateMemStorage(0), CV_POLY_APPROX_DP, cvContourPerimeter(contour) * 0.05, 0);

	//	if (polyContours->total == 4)
	//	{
	//		//获得轮廓外框
	//		CvRect boundingRect = cvBoundingRect(polyContours, 0);
	//		//检查轮廓宽高比
	//		double plate_HWratio = ((double) boundingRect.width) / ((double) boundingRect.height);

	//		if ( boundingRect.width > 80 && boundingRect.height > 40)//plate_HWratio > 2.8 && plate_HWratio < 3.4 &&
	//		{

	//			TRACE("polyContours->total = %d, Area = %d\n", polyContours->total, cvContourArea(polyContours, CV_WHOLE_SEQ));
	//			TRACE("Width = %d, Height = %d, HWratio = %.2f\n", boundingRect.width, boundingRect.height, plate_HWratio);
	//			//将轮廓画出
	//			cvDrawContours(pContourImg, polyContours, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
	//		}
	//	} else {
	//		//TRACE("polyContours->total = %d\n", polyContours->total);
	//	}
	//}



	if (j == ui->m_Current)
	{
		str.Format(_T("摄像头ID：%d\t信号量：%d"), ui->m_pCamInfos[uCamID].nID, Num);
		ui->m_stcXD[i].SetWindowText(str);
		if (ui->m_nIDs[i] != ui->m_pCamInfos[uCamID].nID)
		{
			ui->m_nIDs[i] = ui->m_pCamInfos[uCamID].nID;
			str.Format(_T("%d"), ui->m_pCamInfos[uCamID].nBaseNum);
			ui->m_edtXH[i].SetWindowText(str);
		}

		//cvvImg.CopyOf(pImg);//复制图像到当前的CvvImage对象中  
		//cvvImg.DrawToHDC(ui->m_SrcVideoView.GetDC()->GetSafeHdc(), &rect);
		//cvvImg.Destroy();

		//cvvImg.CopyOf(pContourImg);//复制图像到当前的CvvImage对象中  
		//cvvImg.DrawToHDC(ui->m_ConVideoView.GetDC()->GetSafeHdc(), &rect);
		//cvvImg.Destroy();
	}

	//计算固定值
	if (Num > ui->m_pCamInfos[uCamID].nBaseNum)
	{
		ui->m_pCamInfos[uCamID].nIsCar = 1;
	}else
	{
		ui->m_pCamInfos[uCamID].nIsCar = 0;
	}

	////计算偏移量
	//if (abs(Num - ui->m_pCamInfos[uCamID].nBaseNum) >= ui->m_pCamInfos[uCamID].nOffset)
	//{
	//	ui->m_pCamInfos[uCamID].nIsCar = 1;
	//}else
	//{
	//	ui->m_pCamInfos[uCamID].nIsCar = 0;
	//}


	str.Format(_T("@1B%02d%d&"), i + (j % 8) * 4 + 1, 
		(ui->m_pCamInfos[uCamID].nIsCar == 1 ? 1 : 2));
	if (j > 7)
	{
		ui->m_pPort2->WriteToPort(str);
	}else
	{
		ui->m_pPort1->WriteToPort(str);
	}

	cvReleaseMemStorage( &storage );

	//释放图像
	cvReleaseImage( &pImg );
	cvReleaseImage( &pImgEx );
	cvReleaseImage( &pContourImg );

	delete[] buf;
	buf = NULL;

	delete pCvInfo;
	pCvInfo = NULL;
}

void CTestDlg::DataThread(LPVOID pParam)
{
	CTestDlg *ui = (CTestDlg*)pParam;

	//CWnd *bt = ui->GetDlgItem(IDC_BUTTON42);

	CString str;

	int i, j;//, ch = 1, grop = 1;

	ULONG size=1024*1024;

	CRect rect;
	int result;

	//ui->m_SrcVideoView.GetClientRect(&rect);

	ui->OnButtonInit();
	Sleep(100);
	ui->OnButton2();
	Sleep(500);

	unsigned int uCamNum = g_TotalChannel * 4;
	unsigned int uCarNum = 0;

	int nXDSel;
	int nBinXD = 0, nEndXD = 0;

	while(ui->m_bDataRun)
	{
		nXDSel = ui->m_cboXD.GetCurSel();
		switch(nXDSel)
		{
		case 0:
			nBinXD = 0;
			nEndXD = 4;
			break;
		case 1:
			nBinXD = 0;
			nEndXD = 1;
			break;
		case 2:
			nBinXD = 1;
			nEndXD = 2;
			break;
		case 3:
			nBinXD = 2;
			nEndXD = 3;
			break;
		case 4:
			nBinXD = 3;
			nEndXD = 4;
			break;
		}

		
		for (i = nBinXD; (i < nEndXD) && ui->m_bDataRun; i++)
		{
			str.Format(_T("@1A%d&"), i + 1);
			ui->m_pPort1->WriteToPort(str);
			
			str.Format(_T("@1A%d&"), i + 1);
			ui->m_pPort2->WriteToPort(str);
			Sleep(700);

			for (j = 0; (j < g_TotalChannel) && ui->m_bDataRun; j++)
			{	
				if(g_hDSP[j] != (HANDLE) 0xffffffff)
				{
					result = GetOriginalImage(g_hDSP[j], ui->m_aBuffers[j], &size);

					CV_INFO *pCvInfo = new CV_INFO;
					pCvInfo->i = i;
					pCvInfo->j = j;
					pCvInfo->ui = ui;
					pCvInfo->TempBuffer = ui->m_aBuffers[j];
					if(result == 0)
					{
						::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CVData, pCvInfo, 0, NULL);
					}
				}
			}//End For
		}
			//clock_t start,finish,duration;
			//start=clock();//函数开始时
			//finish=clock();//函数结束时
			//duration=finish-start;
			//CString aa;
			//aa.Format("执行时间  %d",duration);
			//AfxMessageBox(aa);

		//计算车辆总数
		uCarNum = 0;
		for (i = 0; i < uCamNum; i++)
		{
			if(ui->m_pCamInfos[i].nIsCar == 1)
				uCarNum++;
		}

		unsigned int uSYCW = ui->m_LedInfo.uParkingLotNum < uCarNum ? 0 : (ui->m_LedInfo.uParkingLotNum - uCarNum);

		//根据区间设置文本颜色
		CString sTextRgb = _T("R");
		for (i = 0; i < ui->m_LedInfo.TextRgbs.GetCount(); i++)
		{
			if (ui->m_LedInfo.TextRgbs[i]->nMin >= 0 && ui->m_LedInfo.TextRgbs[i]->nMax >= 0)
			{
				if (ui->m_LedInfo.TextRgbs[i]->nMin <= uSYCW
					&& ui->m_LedInfo.TextRgbs[i]->nMax > uSYCW)
				{
					sTextRgb = ui->m_LedInfo.TextRgbs[i]->sRgb;
				}
			}else if (ui->m_LedInfo.TextRgbs[i]->nMin < 0 && ui->m_LedInfo.TextRgbs[i]->nMax >= 0)
			{
				if (ui->m_LedInfo.TextRgbs[i]->nMax > uSYCW)
				{
					sTextRgb = ui->m_LedInfo.TextRgbs[i]->sRgb;
				}
			}else if (ui->m_LedInfo.TextRgbs[i]->nMin >= 0 && ui->m_LedInfo.TextRgbs[i]->nMax < 0)
			{
				if (ui->m_LedInfo.TextRgbs[i]->nMin <= uSYCW)
				{
					sTextRgb = ui->m_LedInfo.TextRgbs[i]->sRgb;
				}
			}else
			{
				sTextRgb = ui->m_LedInfo.TextRgbs[i]->sRgb;
			}
		}

		//根据区间设置文本颜色
		CString sTagRgb = _T("R");
		for (i = 0; i < ui->m_LedInfo.TagRgbs.GetCount(); i++)
		{
			if (ui->m_LedInfo.TagRgbs[i]->nMin >= 0 && ui->m_LedInfo.TagRgbs[i]->nMax >= 0)
			{
				if (ui->m_LedInfo.TagRgbs[i]->nMin <= uSYCW
					&& ui->m_LedInfo.TagRgbs[i]->nMax > uSYCW)
				{
					sTagRgb = ui->m_LedInfo.TagRgbs[i]->sRgb;
				}
			}else if (ui->m_LedInfo.TagRgbs[i]->nMin < 0 && ui->m_LedInfo.TagRgbs[i]->nMax >= 0)
			{
				if (ui->m_LedInfo.TagRgbs[i]->nMax > uSYCW)
				{
					sTagRgb = ui->m_LedInfo.TagRgbs[i]->sRgb;
				}
			}else if (ui->m_LedInfo.TagRgbs[i]->nMin >= 0 && ui->m_LedInfo.TagRgbs[i]->nMax < 0)
			{
				if (ui->m_LedInfo.TagRgbs[i]->nMin <= uSYCW)
				{
					sTagRgb = ui->m_LedInfo.TagRgbs[i]->sRgb;
				}
			}else
			{
				sTagRgb = ui->m_LedInfo.TagRgbs[i]->sRgb;
			}
		}

		ui->m_sNullCarStr.Format(_T("/P%s%01d%s%s%s%03d>"), ui->m_LedInfo.sAddressCode, ui->m_LedInfo.uLocation, ui->m_LedInfo.sDirection, sTagRgb, sTextRgb, uSYCW);
	}
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model, 
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);// device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnButtonInit() 
{
	// TODO: Add your control notification handler code here
	int i;
	g_TotalDSPs = InitDSPs();

	g_TotalChannel = GetTotalChannels();
	if (g_TotalChannel > MAX_VIDEO_CHANNEL)
		g_TotalChannel = MAX_VIDEO_CHANNEL;

	for(i = 0; i < g_TotalChannel; i++)
	{
		g_hDSP[i] = ChannelOpen(i);
	}

	//RECT m_rect;

	//for( i = 0; i < g_TotalChannel; i++)
	//{
	//	if(g_hDSP[i]!= (HANDLE) 0xffffffff)
	//	{
	//		m_pVideoView[i]->GetClientRect(&m_rect);

	//		StartVideoPreview(g_hDSP[i], m_pVideoView[i]->m_hWnd, &m_rect, false, 0, 0);
	//		StopVideoPreview(g_hDSP[i]);
	//	}
	//}

	OnButton3();

	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		TCHAR szFile[256];
		__stprintf(szFile, _T("%s//bk.dll"), strDir);
		SetDefaultDisplayPicture(g_hDSP[m_Current], szFile);
	}
	SYSTEM_BOARD_INFO system_info;
	GetSystemBoardInfo(&system_info);
	GpioInit(1, 0xffffff00);

	if (g_TotalChannel > 0)
	{
		for (int i = 0; i < g_TotalChannel; i++)
		{
			BYTE *TempBuf = new BYTE[1024 * 1024];
			m_aBuffers.Add(TempBuf);
		}

		int nCamSum = g_TotalChannel * 4;
		int nIndex = 0;

		//申请结构的个数
		m_pCamInfos = new CAM_INFO[nCamSum];

		_RecordsetPtr rs=g_DB.GetRecordset(_T("SELECT * FROM CamInfo"));
		while(!rs->adoEOF && nIndex < nCamSum)
		{
			if (rs->GetCollect(_T("ID")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nID = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("ID")));
			}

			if (rs->GetCollect(_T("Effective")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nEffective = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("Effective")));
			}

			if (rs->GetCollect(_T("BaseNum")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nBaseNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("BaseNum")));
			}

			if (rs->GetCollect(_T("Offset")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nOffset = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("Offset")));
			}

			if (rs->GetCollect(_T("RectX")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nRectX = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectX")));
			}

			if (rs->GetCollect(_T("RectY")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nRectY = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectY")));
			}

			if (rs->GetCollect(_T("RectW")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nRectW = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectW")));
			}

			if (rs->GetCollect(_T("RectH")).vt != VT_NULL)
			{
				m_pCamInfos[nIndex].nRectH = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectH")));
			}

			m_pCamInfos[nIndex].nIsCar = 0;

			rs->MoveNext();
			nIndex++;
		}
		rs->Close();

		while (nIndex < nCamSum)
		{
			//给默认值
			m_pCamInfos[nIndex].nID = -1;
			m_pCamInfos[nIndex].nEffective = 1;
			m_pCamInfos[nIndex].nBaseNum = 50;
			m_pCamInfos[nIndex].nOffset = 10;
			m_pCamInfos[nIndex].nRectX = 60;
			m_pCamInfos[nIndex].nRectY = 0;
			m_pCamInfos[nIndex].nRectW = 200;
			m_pCamInfos[nIndex].nRectH = 240;
			m_pCamInfos[nIndex].nIsCar = 0;

			nIndex++;
		}

		rs=g_DB.GetRecordset(_T("SELECT * FROM LedInfo"));
		if(!rs->adoEOF)
		{
			CString sTemp;
			if (rs->GetCollect(_T("AddressCode")).vt != VT_NULL)
			{
				m_LedInfo.sAddressCode = (LPCSTR)(_bstr_t)rs->GetCollect(_T("AddressCode"));
			}

			if (rs->GetCollect(_T("Location")).vt != VT_NULL)
			{
				m_LedInfo.uLocation = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("Location")));
			}

			if (rs->GetCollect(_T("Direction")).vt != VT_NULL)
			{
				m_LedInfo.sDirection = (LPCSTR)(_bstr_t)rs->GetCollect(_T("Direction"));
			}

			if (rs->GetCollect(_T("ParkingLotNum")).vt != VT_NULL)
			{
				m_LedInfo.uParkingLotNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("ParkingLotNum")));
			}

			if (rs->GetCollect(_T("TextFormat")).vt != VT_NULL)
			{
				sTemp = (LPCSTR)(_bstr_t)rs->GetCollect(_T("TextFormat"));
				CString s1, s2;
				int nS1 = 0, nS2 = 0;
				int nMin, nMax;
				CString sRgb;
				while(AfxExtractSubString(s1, sTemp, nS1, ';'))
				{
					while(AfxExtractSubString(s2, s1, nS2, ':'))
					{
						switch(nS2)
						{
						case 0:
							s2 == "" ? nMin = -1 : nMin = _ttoi(s2);
							break;
						case 1:
							s2 == "" ? nMax = -1 : nMax = _ttoi(s2);
							break;
						case 2:
							sRgb = s2;
							break;
						}
						nS2++;
					}
					if (nS2 == 3)
					{
						RGB_INFO *RgbInfo = new RGB_INFO;
						RgbInfo->nMin = nMin;
						RgbInfo->nMax = nMax;
						RgbInfo->sRgb = sRgb;
						m_LedInfo.TextRgbs.Add(RgbInfo);
					}
					nS2 = 0;
					nS1++;
				}
			}

			if (rs->GetCollect(_T("TagFormat")).vt != VT_NULL)
			{
				sTemp = (LPCSTR)(_bstr_t)rs->GetCollect(_T("TagFormat"));
				CString s1, s2;
				int nS1 = 0, nS2 = 0;
				int nMin, nMax;
				CString sRgb;
				while(AfxExtractSubString(s1, sTemp, nS1, ';'))
				{
					while(AfxExtractSubString(s2, s1, nS2, ':'))
					{
						switch(nS2)
						{
						case 0:
							s2 == "" ? nMin = -1 : nMin = _ttoi(s2);
							break;
						case 1:
							s2 == "" ? nMax = -1 : nMax = _ttoi(s2);
							break;
						case 2:
							sRgb = s2;
							break;
						}
						nS2++;
					}
					if (nS2 == 3)
					{
						RGB_INFO *RgbInfo = new RGB_INFO;
						RgbInfo->nMin = nMin;
						RgbInfo->nMax = nMax;
						RgbInfo->sRgb = sRgb;
						m_LedInfo.TagRgbs.Add(RgbInfo);
					}
					nS2 = 0;

					nS1++;
				}
			}
		}else
		{
			m_LedInfo.sAddressCode = _T("80");
			m_LedInfo.uLocation = 1;
			m_LedInfo.sDirection = _T("L");
			m_LedInfo.uParkingLotNum = 0;
		}
		rs->Close();
	}
}

void CTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	int i;

	m_bDataRun = false;

	for (i=0; i<36; i++)
	{
		if (m_pVideoView[i])
		{
			m_pVideoView[i]->CloseWindow();
			delete m_pVideoView[i];
			m_pVideoView[i] = NULL;
		}
	}
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopVideoCapture(g_hDSP[i]);
			StopSaveFile(i);

		}
	}
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopDsp(g_hDSP[i]);
		}
	}
	Sleep(1);
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			ChannelClose(g_hDSP[i]);
		}
	}
	for( i = 0; i<MAX_VIDEO_CHANNEL; i++)
	{
		if(m_pMediaFile[i])
		{
			delete m_pMediaFile[i];
			m_pMediaFile[i] = NULL;
		}
	}

	DeInitDSPs();

	m_pPort1->StopMonitoring();
	m_pPort2->StopMonitoring();
}

inline DWORD MakeAMask( BYTE bWidth )
{
	return ( bWidth >= 32 ? 0 : (DWORD)1 << bWidth ) - 1;
}

void CTestDlg::OnButton2() 
{

	int i;

	for( i = 0; i<g_TotalChannel; i++)
	{
		StartDsp(g_hDSP[i]);
		Set_Main_Enable(g_hDSP[i], 0);
	}

	for( i = 0; i<g_TotalChannel; i++)
	{
		StartDMA(g_hDSP[i]);
	}

}

void CTestDlg::SetByteValue(DWORD Offset, BYTE Start, BYTE Width, BYTE Data)
{
	DWORD Result = 0x00;
	DWORD dwMask = MakeAMask(Width);
	dwMask = dwMask << Start;
	DWORD dwFieldValue = Data << Start;
	dwFieldValue &= dwMask;

	DWORD dwRegContent = Result;
	dwRegContent &= ~dwMask;
	dwRegContent |= dwFieldValue;

	CString strText;
	strText.Format(_T("0x%x"), dwRegContent);

}

BYTE CTestDlg::GetByteValue(DWORD Offset, BYTE Start, BYTE Width)
{
	BYTE Result = 0x24;
	BYTE dwValue = ( Result >> Start );

	BYTE dwMask = (BYTE)MakeAMask(Width);
	dwValue = (dwValue & dwMask);
	return dwValue;
}

void CTestDlg::OnReleasedcaptureSliderReat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CTestDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	if(slider == &m_Slider_FrameRate)
	{
		int nPos = m_Slider_FrameRate.GetPos();
		SetDlgItemInt(IDC_STATIC_FRAMERATE, nPos);
		SetFrameRate(nPos);
	}
	if(slider == &m_Slider_BR)
	{
		if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff )
		{
			int nBr = m_Slider_BR.GetPos();
			int nCo = m_Slider_CO.GetPos();
			int nSa = m_Slider_SA.GetPos();
			int nHu = m_Slider_HU.GetPos();
			SetVideoPara(g_hDSP[m_Current], nBr, nCo, nSa, nHu);
			SetDlgItemInt(IDC_STATIC1, nBr);
		}
	}
	if(slider == &m_Slider_CO)
	{
		if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff )
		{
			int nBr = m_Slider_BR.GetPos();
			int nCo = m_Slider_CO.GetPos();
			int nSa = m_Slider_SA.GetPos();
			int nHu = m_Slider_HU.GetPos();
			SetVideoPara(g_hDSP[m_Current], nBr, nCo, nSa, nHu);
			SetDlgItemInt(IDC_STATIC2, nCo);
		}
	}
	if(slider == &m_Slider_SA)
	{
		if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff )
		{
			int nBr = m_Slider_BR.GetPos();
			int nCo = m_Slider_CO.GetPos();
			int nSa = m_Slider_SA.GetPos();
			int nHu = m_Slider_HU.GetPos();
			SetVideoPara(g_hDSP[m_Current], nBr, nCo, nSa, nHu);
			SetDlgItemInt(IDC_STATIC3, nSa);
		}
	}
	if(slider == &m_Slider_HU)
	{
		if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff )
		{
			int nBr = m_Slider_BR.GetPos();
			int nCo = m_Slider_CO.GetPos();
			int nSa = m_Slider_SA.GetPos();
			int nHu = m_Slider_HU.GetPos();
			SetVideoPara(g_hDSP[m_Current], nBr, nCo, nSa, nHu);
			SetDlgItemInt(IDC_STATIC4, nHu);
		}
	}
	if(slider == &m_Slider_SH)
	{
		if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff )
		{

			int nSH = m_Slider_SH.GetPos();
			SetVideoSharp(g_hDSP[m_Current], nSH);
			SetDlgItemInt(IDC_STATIC5, nSH);
		}
	}


	if(slider == &m_Slider_Volume)
	{
		if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff )
		{
			int nVolume = m_Slider_Volume.GetPos();
			SetAudioVolume(g_hDSP[m_Current], nVolume);
		}
	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTestDlg::SetFrameRate(int nFrameRate)
{
	for(int i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			SetVideoFrameRate(g_hDSP[i], nFrameRate);
		}
	}
}

void CTestDlg::SendAndSaveData(int nChannel)
{
	if(nChannel>=0 && nChannel<MAX_VIDEO_CHANNEL)
	{
		m_framerate[nChannel]++;
		m_nFrameRate++;
	}
}

void CTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(TIMER_MAIN == nIDEvent)
	{
		CTime time = CTime::GetCurrentTime();
		ShowResult();
		ChangeRecord(time);   
	}

	CDialog::OnTimer(nIDEvent);
}

void CTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	//	StopAllDSPs();

	CDialog::OnOK();
}

void CTestDlg::OnButton3() 
{
	//StopAllDSPs();
	for(int nChannel = 0; nChannel<g_TotalChannel; nChannel++)
	{
		if(g_hDSP[nChannel]!= (HANDLE) 0xffffffff)
		{
			int Size = m_Combo2.GetCurSel();
			if(nChannel ==0)
			{
				//Size = 4;
			}


			int standard = m_Combo1.GetCurSel();
			if(standard == 0)
			{
				SetVideoStandard(g_hDSP[nChannel], StandardPAL);
			}
			else
			{
				SetVideoStandard(g_hDSP[nChannel], StandardNTSC);
			}

			switch(Size)
			{
			case 0:
				SetEncoderPictureFormat(g_hDSP[nChannel], ENC_QCIF_FORMAT);
				break;
			case 1:
				SetEncoderPictureFormat(g_hDSP[nChannel], ENC_DCIF_FORMAT);
				break;
			case 2:
				SetEncoderPictureFormat(g_hDSP[nChannel], ENC_CIF_FORMAT);
				break;
			case 3:
				SetEncoderPictureFormat(g_hDSP[nChannel], ENC_SVGA_FORMAT);
				break;
			case 4:
				SetEncoderPictureFormat(g_hDSP[nChannel], ENC_2CIF_FORMAT);
				break;

			default:
				SetEncoderPictureFormat(g_hDSP[nChannel], ENC_CIF_FORMAT);
				break;

			}

			int nPos = m_Slider_FrameRate.GetPos();
			if(nChannel ==0)
			{
				//nPos = 12;
			}		
			SetFrameRate(nPos);

			SetOsd(g_hDSP[nChannel], false);



			int nBr = m_Slider_BR.GetPos();
			int nCo = m_Slider_CO.GetPos();
			int nSa = m_Slider_SA.GetPos();
			int nHu = m_Slider_HU.GetPos();
			SetVideoPara(g_hDSP[nChannel], nBr, nCo, nSa, nHu);

			int nSH = m_Slider_SH.GetPos();
			SetVideoSharp(g_hDSP[nChannel], nSH);

			char szChannelName[64];
			sprintf(szChannelName, "Channel  %02d", nChannel+1);
			USHORT Buf1[100];

			USHORT Buf2[100]={10, 20, _OSD_YEAR4, '-', _OSD_MONTH2, '-', _OSD_DAY, ' ', _OSD_HOUR24, ':', _OSD_MINUTE, ':', _OSD_SECOND, ' ', '\0'};
			Buf2[0] = 10;
			Buf2[1] = 40;
			int Length;
			int offset;
			char szTemp;
			char szTemp1;
			int result;
			Length = 0;
			offset= 0;
			szTemp= NULL;
			szTemp1= NULL;
			Buf1[0]=10;
			Buf1[1]=10;
			Length  = strlen(szChannelName);

			WCHAR wszOsdSting[256];        // Unicode user name
			int nCount = MultiByteToWideChar( CP_ACP, 0, szChannelName, 
				Length, wszOsdSting, 
				sizeof(wszOsdSting)/sizeof(wszOsdSting[0]) );

			offset=2;
			for (int temp=0;temp<nCount;)
			{
				Buf1[offset]=wszOsdSting[temp];
				offset++;
				temp++;
			}
			Buf1[offset]='\0';
			if(nChannel ==0)
			{
				SetStreamType(g_hDSP[nChannel], STREAM_TYPE_AVSYNC);
			}			
			result=SetOsdDisplayMode(g_hDSP[nChannel], 255, FALSE, 0x1000, Buf1, Buf2);

		}
	}
}

LRESULT CTestDlg::OnMsgViewDbClick(WPARAM wParam, LPARAM lParam)
{
	RECT m_rect;
	m_bOne = !m_bOne;
	int i;

	if(m_bOne)
	{
		for( i = 0; i<MAX_VIDEO_CHANNEL; i++)
		{
			m_pVideoView[i]->MoveWindow(CRect(0, 0, 0, 0));
		}
		m_pVideoView[m_Current]->MoveWindow(&m_Onerect);
	}
	else
	{
		ShowHideScreen();
	}

	for( i = 0; i<g_TotalChannel; i++)
	{

		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			m_pVideoView[i]->GetClientRect(&m_rect);
			StartVideoPreview(g_hDSP[i], m_pVideoView[i]->m_hWnd, &m_rect, false, 0, 0);
		}
	}
	UpdateVideoPreview();

	return 0;
}

LRESULT CTestDlg::OnMsgViewClick(WPARAM wParam, LPARAM lParam)
{
	m_Current = wParam;
	return 0;
}

void CTestDlg::InitRect()
{
	m_Current = 0;
	m_bOne = FALSE;
	for(int i = 0; i<MAX_VIDEO_CHANNEL; i++)
	{
		m_pVideoView[i]->m_nIndex = i;
	}
	CRect rect;
	this->GetClientRect(&rect);

	m_Onerect.left = 5;
	m_Onerect.top = 5;
	m_Onerect.right = (rect.bottom - 4) / 0.75;
	m_Onerect.bottom = rect.bottom - 4;

	//m_SrcVideoView.Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + 100);
	//m_ConVideoView.Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + 100);

	this->GetDlgItem(IDC_BUTTON2X2)->MoveWindow(m_Onerect.right + 10,  10, 50, 50);
	this->GetDlgItem(IDC_BUTTON3X3)->MoveWindow(m_Onerect.right + 70,  10, 50, 50);
	this->GetDlgItem(IDC_BUTTON4X4)->MoveWindow(m_Onerect.right + 130, 10, 50, 50);
	this->GetDlgItem(IDC_BUTTON6X6)->MoveWindow(m_Onerect.right + 190, 10, 50, 50);

	//m_SrcVideoView.MoveWindow(m_Onerect.right + 10,  130, rect.right - m_Onerect.right - 20, (rect.right - m_Onerect.right - 20) * 0.75);
	//CRect rectS;
	//m_SrcVideoView.GetWindowRect(rectS);
	//rectS.OffsetRect(0, rectS.Height() + 10);

	//m_ConVideoView.MoveWindow(rectS);


	//this->GetDlgItem(IDC_BUTTON42)->MoveWindow(m_Onerect.right + 10, 70, rect.right - m_Onerect.right - 20, 50);

	this->GetDlgItem(IDOK)->MoveWindow(m_Onerect.right + 10, rect.bottom - 60, rect.right - m_Onerect.right - 20, 50);

	m_stcXD[0].MoveWindow(m_Onerect.right + 50, 90, 160, 25);
	m_stcXD[0].SetWindowText(_T("摄像头ID：0\t信号量：0"));
	m_stcXD[1].MoveWindow(m_Onerect.right + 50, 130, 160, 25);
	m_stcXD[1].SetWindowText(_T("摄像头ID：0\t信号量：0"));
	m_stcXD[2].MoveWindow(m_Onerect.right + 50, 170, 160, 25);
	m_stcXD[2].SetWindowText(_T("摄像头ID：0\t信号量：0"));
	m_stcXD[3].MoveWindow(m_Onerect.right + 50, 210, 160, 25);
	m_stcXD[3].SetWindowText(_T("摄像头ID：0\t信号量：0"));

	m_cboXD.MoveWindow(m_Onerect.right + 50, 250, 120, 125);

	m_edtXH[0].MoveWindow(m_Onerect.right + 210, 90, 80, 25);
	m_edtXH[1].MoveWindow(m_Onerect.right + 210, 130, 80, 25);
	m_edtXH[2].MoveWindow(m_Onerect.right + 210, 170, 80, 25);
	m_edtXH[3].MoveWindow(m_Onerect.right + 210, 210, 80, 25);

	m_btnBC.MoveWindow(m_Onerect.right + 210, 250, 80, 25);
	m_btnBC.SetWindowTextA(_T("保存"));

	m_btnYL.MoveWindow(m_Onerect.right + 50, 290, 120, 125);
	m_btnYL.SetWindowText(_T("开启预览"));
}

void CTestDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[0]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[0]);
	}
}

void CTestDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[1]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[1]);
	}	
}

void CTestDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[2]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[2]);
	}	
}

void CTestDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[3]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[3]);
	}	
}

void CTestDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[4]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[4]);
	}	
}

void CTestDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[0]!= (HANDLE) 0xffffffff)
	{
		Test(g_hDSP[0]);
	}	
}

void CTestDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[1]!= (HANDLE) 0xffffffff)
	{
		Test(g_hDSP[1]);
	}	
}

void CTestDlg::OnButton11() 
{
	if(g_hDSP[2]!= (HANDLE) 0xffffffff)
	{
		Test(g_hDSP[2]);
	}	
}

void CTestDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[3]!= (HANDLE) 0xffffffff)
	{
		Test(g_hDSP[3]);
	}	
}

void CTestDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[4]!= (HANDLE) 0xffffffff)
	{
		Test(g_hDSP[4]);
	}	
}

void CTestDlg::OnButton25() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[5]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[5]);
	}
}

void CTestDlg::OnButton26() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[6]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[6]);
	}	
}

void CTestDlg::OnButton27() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[7]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[7]);
	}	
}

void CTestDlg::OnButton28() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[8]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[8]);
	}	
}

void CTestDlg::OnButton29() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[9]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[9]);
	}	
}

void CTestDlg::OnButton30() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[10]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[10]);
	}	
}

void CTestDlg::OnButton31() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[11]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[11]);
	}	
}

void CTestDlg::OnButton32() 
{
	if(g_hDSP[12]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[12]);
	}	
}

void CTestDlg::OnButton33() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[13]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[13]);
	}	
}

void CTestDlg::OnButton34() 
{
	if(g_hDSP[14]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[14]);
	}

}

void CTestDlg::OnButton35() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[15]!= (HANDLE) 0xffffffff)
	{
		StartDsp(g_hDSP[15]);
	}	
}

void CTestDlg::OnCloseupCombo2() 
{
	// TODO: Add your control notification handler code here

}

void CTestDlg::OnCloseupCombo1() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_Combo2.GetCurSel();
	if(nIndex == 0)
	{
		m_Slider_FrameRate.SetRange( 0, 25, TRUE);
		m_Slider_FrameRate.SetTicFreq(1);
		m_Slider_FrameRate.SetPageSize(1);
	}
	else
	{
		m_Slider_FrameRate.SetRange( 0, 30, TRUE);
		m_Slider_FrameRate.SetTicFreq(1);
		m_Slider_FrameRate.SetPageSize(1);

	}	
}

void CTestDlg::StreamDirectRead(ULONG i, void *DataBuf, DWORD Length, int FrameType)
{


	if(FrameType == PktIFrames && !m_bCanWriteFile[i] && m_pMediaFile[i]&&m_bRecord[i])
	{
		m_bCanWriteFile[i] = 1;
	}
	if(m_bCanWriteFile[i])
	{
		if(m_bRecord[i])
		{

			if(FrameType ==PktIFrames || FrameType ==PktPFrames ||FrameType ==PktBBPFrames)
			{
				if(m_pMediaFile[i] && m_pMediaFile[i]->bWrite)
				{
					//				TRACE(_T("FrameType %d  Length %d\n"), FrameType, Length);
					m_pMediaFile[i]->Write((char*)DataBuf, Length);
				}
				/*
				m_pAviFile[i]->WriteVideo((BYTE*)DataBuf, Length, FrameType==PktIFrames?1:0, false);
				*/
				if(i ==0)
					lBitRateValue+=Length;

			}
			if(FrameType ==PktAudioFrames)
			{
				if(m_pMediaFile[i] && m_pMediaFile[i]->bWrite)
				{

					m_pMediaFile[i]->Write((char*)DataBuf, Length);

				}
			}
		}
	}



}

void CTestDlg::StartSaveFile(int nChannel, TCHAR *pFileName, DWORD nFrameRate, int nWidth, int nHeight)
{
	DWORD dwResult =0;
	if(nChannel>=0 && nChannel<MAX_VIDEO_CHANNEL)
	{
		if(g_hDSP[nChannel]!= (HANDLE) 0xffffffff)
		{
			if(m_pMediaFile[nChannel])
			{
				m_pMediaFile[nChannel]->Open(pFileName);
				m_pMediaFile[nChannel]->bWrite = true;
				m_bRecord[nChannel] = true;
				m_bCanWriteFile[nChannel] = false;
			}
			/*
			if(m_pAviFile[nChannel])
			{
			delete m_pAviFile[nChannel];
			m_pAviFile[nChannel] = NULL;
			}
			m_pAviFile[nChannel] = new CAviFileObj();
			if(m_pAviFile[nChannel])
			{
			m_pAviFile[nChannel]->SetVideoFormat(streamtypeVIDEO, nWidth, nHeight, nFrameRate, 1, &dwResult);
			m_pAviFile[nChannel]->SetAudioFormat(0, &dwResult);
			m_pAviFile[nChannel]->Create(pFileName, OF_SHARE_COMPAT|OF_CREATE| OF_WRITE);
			m_bRecord[nChannel] = true;
			}
			*/
		}
	}
}

void CTestDlg::StopSaveFile(int nChannel)
{
	if(nChannel>=0 && nChannel<MAX_VIDEO_CHANNEL)
	{
		m_bRecord[nChannel] = false;
		DWORD dwResult =0;
		m_bCanWriteFile[nChannel] = false;
		if(m_pMediaFile[nChannel])
		{
			m_pMediaFile[nChannel]->bWrite= false;
			m_pMediaFile[nChannel]->Close();
		}
		/*
		if(m_pAviFile[nChannel])
		{
		m_pAviFile[nChannel]->Close(&dwResult);
		delete m_pAviFile[nChannel];
		m_pAviFile[nChannel] = NULL;
		}
		*/
	}
}

void CTestDlg::ChangeRecord(CTime time)
{
	int nIndex = 0;
	int nHour = 0;
	int nMinute = 0;
	int nSec = 0;
	int nTemp = 0;
	int i = 0;
	nHour = time.GetHour();
	nMinute = time.GetMinute();
	nSec= time.GetSecond();
	TCHAR szFileName[256];
	if((nMinute == 30 || nMinute == 0) && nSec == 0)
	{
		for( i = 0; i<g_TotalChannel; i++)
		{
			StopSaveFile(i);
		}
		for( i = 0; i<g_TotalChannel; i++)
		{
			__stprintf(szFileName, _T("d://Record//%02d%04d%02d%02d%02d%02d%02d.mp4"), i+1, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			int Size = m_Combo2.GetCurSel();
			int standard = m_Combo1.GetCurSel();

			int nPos = m_Slider_FrameRate.GetPos();
			PictureFormat_t pictureformat;
			VideoStandard_t  m_videostandard;

			if(standard == 0)
			{
				m_videostandard = StandardPAL;

			}
			else
			{
				m_videostandard = StandardNTSC;

			}
			switch(Size)
			{
			case 0:
				pictureformat = ENC_QCIF_FORMAT;

				break;
			case 1:
				pictureformat = ENC_DCIF_FORMAT;

				break;
			case 2:
				pictureformat = ENC_CIF_FORMAT;

				break;
			case 3:
				pictureformat = ENC_SVGA_FORMAT;

				break;
			case 4:
				pictureformat = ENC_2CIF_FORMAT;

				break;

			default:
				pictureformat = ENC_CIF_FORMAT;
				break;

			}
			CRect rect;
			switch(pictureformat)
			{
			case ENC_CIF_FORMAT		    : // 352*288 or 352*240
				{
					switch(m_videostandard)
					{
					case  StandardNone:
						rect  = CRect(0, 0, 352, 288);
						break;
					case StandardNTSC:
						rect  = CRect(0, 0, 352, 240);
						break;
					case StandardPAL:
						rect  = CRect(0, 0, 352, 288);
						break;
					case StandardSECAM:
						rect  = CRect(0, 0, 300, 200);
						break;
					default:
						return ;
						break;
					}
				}
				break;
			case ENC_QCIF_FORMAT		: // 176*144 or 176*120
				{
					switch(m_videostandard)
					{
					case  StandardNone:
						rect  = CRect(0, 0, 176, 144);
						break;
					case StandardNTSC:
						rect  = CRect(0, 0, 176, 120);
						break;
					case StandardPAL:
						rect  = CRect(0, 0, 304, 208);
						break;
					case StandardSECAM:
						rect  = CRect(0, 0, 176, 120);
						break;
					default:
						return ;
					}
				}
				break;
			case ENC_2CIF_FORMAT		: // 704*288 or 704*240
				{
					switch(m_videostandard)
					{
					case  StandardNone:
						rect  = CRect(0, 0, 704, 288);
						break;
					case StandardNTSC:
						rect  = CRect(0, 0, 704, 240);
						break;
					case StandardPAL:
						rect  = CRect(0, 0, 704, 288);
						break;
					case StandardSECAM:
						rect  = CRect(0, 0, 704, 240);
						break;
					default:
						return ;
					}
				}
				break;
			case ENC_4CIF_FORMAT		: // 704*576 or 704*480
				{
					switch(m_videostandard)
					{
					case  StandardNone:
						rect  = CRect(0, 0, 704, 576);
						break;
					case StandardNTSC:
						rect  = CRect(0, 0, 704, 480);
						break;
					case StandardPAL:
						rect  = CRect(0, 0, 704, 576);
						break;
					case StandardSECAM:
						rect  = CRect(0, 0, 704, 480);
						break;
					default:
						return ;
					}
				}
				break;
			case ENC_SCIF_FORMAT		: // 240*192 or 240*164
				{
					switch(m_videostandard)
					{
					case  StandardNone:
						rect  = CRect(0, 0, 240, 192);
						break;
					case StandardNTSC:
						rect  = CRect(0, 0, 240, 164);
						break;
					case StandardPAL:
						rect  = CRect(0, 0, 240, 192);
						break;
					case StandardSECAM:
						rect  = CRect(0, 0, 240, 164);
						break;
					default:
						return ;
					}
				}

				break;
			case ENC_SVGA_FORMAT		: // 640*288 or 640*240
				{	
					switch(m_videostandard)
					{
					case  StandardNone:
						rect  = CRect(0, 0, 640, 288);
						break;
					case StandardNTSC:
						rect  = CRect(0, 0, 640, 240);
						break;
					case StandardPAL:
						rect  = CRect(0, 0, 640, 288);
						break;
					case StandardSECAM:
						rect  = CRect(0, 0, 640, 240);
						break;
					default:
						return ;
					}
				}
				break;
			case ENC_VGA_FORMAT		    : // 640*480
				{
					rect  = CRect(0, 0, 640, 480);
				}
				break;
			case ENC_DCIF_FORMAT		: // 320*240
				{
					rect  = CRect(0, 0, 320, 240);
				}
				break;
			default:
				return ;
			}
			StartSaveFile(i, szFileName, nPos/(m_nSubChannel+1), rect.Width(), rect.Height());
		}	
	}
}


#define VIDEO_CLEARANCE 1
void CTestDlg::ShowHideScreen()
{
	int nIndex = 0;
	CRect rect, m_rOneVid(m_Onerect);
	int i, j, offsetX, offsetY;
	SIZE size;

	size.cx = (m_rOneVid.Width()-VIDEO_CLEARANCE*(m_nViewMode -1))/m_nViewMode;
	size.cy = (m_rOneVid.Height()-VIDEO_CLEARANCE*(m_nViewMode -1))/m_nViewMode;

	CRect rectOne = CRect(m_rOneVid.TopLeft(), size);
	offsetX = size.cx + VIDEO_CLEARANCE;
	offsetY = size.cy + VIDEO_CLEARANCE;


	for (i=0; i<m_nViewMode; i++)
	{
		for (j = 0;j<m_nViewMode;j++)
		{
			rect = rectOne;
			rect.OffsetRect(offsetX*j, offsetY*i);
			m_pVideoView[nIndex]->ShowWindow(SW_HIDE);
			m_pVideoView[nIndex]->MoveWindow(rect);
			m_pVideoView[nIndex]->ShowWindow(SW_SHOW);// show

			nIndex++;
		}
	}

	for (;nIndex < MAX_VIDEO_CHANNEL;nIndex++)
	{
		m_pVideoView[nIndex]->MoveWindow(CRect(0, 0, 0, 0));
		m_pVideoView[nIndex]->ShowWindow(SW_HIDE);// hide

	}

	UpdateVideoPreview();
}

void CTestDlg::OnButton2x2() 
{
	m_nViewMode = 2;
	ShowHideScreen();
}

void CTestDlg::OnButton3x3() 
{
	m_nViewMode = 3;
	ShowHideScreen();
}

void CTestDlg::OnButton4x4() 
{
	m_nViewMode = 4;
	ShowHideScreen();
}

void CTestDlg::OnButton6x6() 
{
	m_nViewMode = 6;
	ShowHideScreen();
}

void CTestDlg::OnButton38() 
{
	// TODO: Add your control notification handler code here
	CTime time = CTime::GetCurrentTime();
	//	ReStartAllDSPs();
	int i;
	TCHAR szFileName[256];
	for( i = 0; i<g_TotalChannel; i++)
	{
		m_bCanWriteFile[i] = false;
		StartVideoCapture(g_hDSP[i]);
		__stprintf(szFileName, _T("d://Record//%02d%04d%02d%02d%02d%02d%02d.mp4"), i+1, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		int Size = m_Combo2.GetCurSel();
		int standard = m_Combo1.GetCurSel();

		int nPos = m_Slider_FrameRate.GetPos();
		PictureFormat_t pictureformat;
		VideoStandard_t  m_videostandard;

		if(standard == 0)
		{
			m_videostandard = StandardPAL;

		}
		else
		{
			m_videostandard = StandardNTSC;

		}
		switch(Size)
		{
		case 0:
			pictureformat = ENC_QCIF_FORMAT;

			break;
		case 1:
			pictureformat = ENC_DCIF_FORMAT;

			break;
		case 2:
			pictureformat = ENC_CIF_FORMAT;

			break;
		case 3:
			pictureformat = ENC_SVGA_FORMAT;

			break;
		case 4:
			pictureformat = ENC_2CIF_FORMAT;

			break;

		default:
			pictureformat = ENC_CIF_FORMAT;
			break;

		}
		CRect rect;
		switch(pictureformat)
		{
		case ENC_CIF_FORMAT		    : // 352*288 or 352*240
			{
				switch(m_videostandard)
				{
				case  StandardNone:
					rect  = CRect(0, 0, 352, 288);
					break;
				case StandardNTSC:
					rect  = CRect(0, 0, 352, 240);
					break;
				case StandardPAL:
					rect  = CRect(0, 0, 352, 288);
					break;
				case StandardSECAM:
					rect  = CRect(0, 0, 300, 200);
					break;
				default:
					return ;
					break;
				}
			}
			break;
		case ENC_QCIF_FORMAT		: // 176*144 or 176*120
			{
				switch(m_videostandard)
				{
				case  StandardNone:
					rect  = CRect(0, 0, 176, 144);
					break;
				case StandardNTSC:
					rect  = CRect(0, 0, 176, 120);
					break;
				case StandardPAL:
					rect  = CRect(0, 0, 304, 224);
					break;
				case StandardSECAM:
					rect  = CRect(0, 0, 176, 120);
					break;
				default:
					return ;
				}
			}
			break;
		case ENC_2CIF_FORMAT		: // 704*288 or 704*240
			{
				switch(m_videostandard)
				{
				case  StandardNone:
					rect  = CRect(0, 0, 704, 288);
					break;
				case StandardNTSC:
					rect  = CRect(0, 0, 704, 240);
					break;
				case StandardPAL:
					rect  = CRect(0, 0, 704, 288);
					break;
				case StandardSECAM:
					rect  = CRect(0, 0, 704, 240);
					break;
				default:
					return ;
				}
			}
			break;
		case ENC_4CIF_FORMAT		: // 704*576 or 704*480
			{
				switch(m_videostandard)
				{
				case  StandardNone:
					rect  = CRect(0, 0, 704, 576);
					break;
				case StandardNTSC:
					rect  = CRect(0, 0, 704, 480);
					break;
				case StandardPAL:
					rect  = CRect(0, 0, 704, 576);
					break;
				case StandardSECAM:
					rect  = CRect(0, 0, 704, 480);
					break;
				default:
					return ;
				}
			}
			break;
		case ENC_SCIF_FORMAT		: // 240*192 or 240*164
			{
				switch(m_videostandard)
				{
				case  StandardNone:
					rect  = CRect(0, 0, 240, 192);
					break;
				case StandardNTSC:
					rect  = CRect(0, 0, 240, 164);
					break;
				case StandardPAL:
					rect  = CRect(0, 0, 240, 192);
					break;
				case StandardSECAM:
					rect  = CRect(0, 0, 240, 164);
					break;
				default:
					return ;
				}
			}

			break;
		case ENC_SVGA_FORMAT		: // 640*288 or 640*240
			{	
				switch(m_videostandard)
				{
				case  StandardNone:
					rect  = CRect(0, 0, 640, 288);
					break;
				case StandardNTSC:
					rect  = CRect(0, 0, 640, 240);
					break;
				case StandardPAL:
					rect  = CRect(0, 0, 640, 288);
					break;
				case StandardSECAM:
					rect  = CRect(0, 0, 640, 240);
					break;
				default:
					return ;
				}
			}
			break;
		case ENC_VGA_FORMAT		    : // 640*480
			{
				rect  = CRect(0, 0, 640, 480);
			}
			break;
		case ENC_DCIF_FORMAT		: // 320*240
			{
				rect  = CRect(0, 0, 320, 240);
			}
			break;
		default:
			return ;
		}
		StartSaveFile(i, szFileName, nPos/(m_nSubChannel+1), rect.Width(), rect.Height());
	}	
	m_bCapture = 1;
	EnableCaptureButton();
}

void CTestDlg::ShowResult()
{
	SetDlgItemInt(IDC_STATIC_FRAME_REATE, m_nFrameRate);
	m_nFrameRate = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_1, m_framerate[0]);
	m_framerate[0] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_2, m_framerate[1]);
	m_framerate[1] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_3, m_framerate[2]);
	m_framerate[2] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_4, m_framerate[3]);
	m_framerate[3] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_5, m_framerate[4]);
	m_framerate[4] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_6, m_framerate[5]);
	m_framerate[5] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_7, m_framerate[6]);
	m_framerate[6] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_8, m_framerate[7]);
	m_framerate[7] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_9, m_framerate[8]);
	m_framerate[8] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_10, m_framerate[9]);
	m_framerate[9] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_11, m_framerate[10]);
	m_framerate[10] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_12, m_framerate[11]);
	m_framerate[11] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_13, m_framerate[12]);
	m_framerate[12] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_14, m_framerate[13]);
	m_framerate[13] = 0;
	SetDlgItemInt(IDC_STATIC_SHOW_15, m_framerate[14]);
	m_framerate[14] = 0;

	SetDlgItemInt(IDC_STATIC_SHOW_16, m_framerate[15]);
	m_framerate[15] = 0;

	SetDlgItemInt(IDC_STATIC_SHOW_17, m_framerate[16]);
	m_framerate[16] = 0;

	SetDlgItemInt(IDC_STATIC_SHOW_18, m_framerate[17]);
	m_framerate[17] = 0;

	SetDlgItemInt(IDC_STATIC_SHOW_19, m_framerate[18]);
	m_framerate[18] = 0;

	SetDlgItemInt(IDC_STATIC_SHOW_20, m_framerate[19]);
	m_framerate[19] = 0;


	float	fTemp = 0.0;
	CString strText;
	fTemp = (float)(lBitRateValue / 1000);
	strText.Format(_T("%.01f kbps"), fTemp);
	SetDlgItemText(IDC_STATIC_FRAME_FPS, strText);
	lBitRateValue = 0;
}

void CTestDlg::OnButton39() 
{
	// TODO: Add your control notification handler code here
	//ULONG BoardType;
	BYTE bTempSerBuf[128];
	memset(bTempSerBuf, 0, sizeof(bTempSerBuf));
	CString strText;
	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		//	GetBoardInfoEx(g_hDSP[m_Current], &BoardType, bTempSerBuf);
		/*	
		strText.Format("%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x", 
		bTempSerBuf[0], bTempSerBuf[1], bTempSerBuf[2], bTempSerBuf[3], bTempSerBuf[4], bTempSerBuf[5], 
		bTempSerBuf[6], bTempSerBuf[7], bTempSerBuf[8], bTempSerBuf[9], bTempSerBuf[10], bTempSerBuf[11], 
		bTempSerBuf[12], bTempSerBuf[13], bTempSerBuf[14], bTempSerBuf[15]);*/
		//AfxMessageBox(strText);
		//		AfxMessageBox((char*)bTempSerBuf);

		Test(g_hDSP[m_Current]);
	}

}

void CTestDlg::OnButton40() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		SetAudioPreview(g_hDSP[m_Current], true);
	}
}

void CTestDlg::EnableCaptureButton()
{
	GetDlgItem(IDC_BUTTON38)->EnableWindow(!m_bCapture);
	GetDlgItem(IDC_BUTTON41)->EnableWindow(m_bCapture);
}

void CTestDlg::OnButton41() 
{
	// TODO: Add your control notification handler code here
	m_bCapture = 0;
	int i;
	//TCHAR szFileName[256];
	for( i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopVideoCapture(g_hDSP[i]);
			StopSaveFile(i);
		}
	}
	m_bCapture = 0;
	EnableCaptureButton();
}

void CTestDlg::OnButton42() 
{
	// TODO: Add your control notification handler code here
	char szFile[256];
	CTime time = CTime::GetCurrentTime();
	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		ULONG size=1024*1024;
		BYTE *TempBuffer=new BYTE[size];
		int result;
		if(TempBuffer)
		{
			result = GetOriginalImage(g_hDSP[m_Current], TempBuffer, &size);
			if(result == 0)
			{
				sprintf(szFile, "c:\\%02d%02d%02d%02d%02d.jpg", m_Current+1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
				ConvertYUVToJpgFile(szFile, TempBuffer);

			}
			else
			{
				AfxMessageBox(_T("fd"));
			}

			delete [] TempBuffer;
			TempBuffer = NULL;
		}
	}	
}

int CTestDlg::ConvertYUVToJpgFile(char *pszFile, BYTE *pSource)
{
	//BOOL bSize = 0;
	//if(pSource!= NULL )
	//{

	//	int nWidth, nHeight;
	//	nWidth = *(short *)pSource;
	//	nHeight = *(short *)(pSource + 2);
	//	BYTE *source = pSource + 4;

	//	int nSize = 3 * nWidth * nHeight;
	//	BYTE *buf = new BYTE [nSize];
	//	BYTE *dest1 = buf;//+nSize-nWidth*3;

	//	if(buf)
	//	{
	//		int w, h;
	//		double R, G, B, y1, u1, v1, y2, u2, v2;
	//		for (h=0; h<nHeight; h++)
	//		{

	//			for (w=0;w<nWidth/2;w++)
	//			{
	//				y1 = source[0];
	//				y2 = source[2];
	//				v1 = v2 = source[3]-128;
	//				//v2 = source[3]-128;
	//				u1 = u2 = source[1]-128;
	//				//u2 = source[1]-128;

	//				R = (double)(y1 + 1.375 * v1);
	//				G = (double)(y1 - 0.34375 * u1 - 0.703125 * v1);
	//				B = (double)(y1 + 1.734375 * u1);
	//				R = max (0, min (255, R));
	//				G = max (0, min (255, G));
	//				B = max (0, min (255, B));

	//				dest1[0] = (BYTE)B;
	//				dest1[1] = (BYTE)G;
	//				dest1[2] = (BYTE)R;
	//				dest1 += 3;

	//				R = (double)(y2 + 1.375 * v1);
	//				G = (double)(y2 - 0.34375 * u1 - 0.703125 * v1);
	//				B = (double)(y2 + 1.734375 * u1);
	//				R = max (0, min (255, R));
	//				G = max (0, min (255, G));
	//				B = max (0, min (255, B));

	//				dest1[0] = (BYTE)B;
	//				dest1[1] = (BYTE)G;
	//				dest1[2] = (BYTE)R;
	//				dest1 += 3;

	//				source += 4;
	//			}
	//			//dest1 -= 2*nWidth*3;
	//		}



	//		IplImage* pImg = NULL;
	//		IplImage* pContourImg = NULL;  
	//		CvMemStorage * storage = cvCreateMemStorage(0);  
	//		CvSeq * contour = 0;  
	//		//cvNamedWindow("src", 1);  
	//		//cvNamedWindow("contour",1);  
	//		//载入图像，强制转化为Gray 

	//		if(pImg = cvCreateImage( cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3 ))
	//		//if( (pImg = cvLoadImage( pszFile, 0)) != 0 )
	//		{
	//			memcpy(pImg->imageData, buf, nSize);
	//			CRect rect;
	//			m_SrcVideoView.GetClientRect(&rect);//将CWind类客户区的坐标点传给矩形

	//			//HDC hDC = m_SrcVideoView.GetDC()->GetSafeHdc();

	//			CvvImage cvvImg; //CvvImage类
	//			cvvImg.CopyOf(pImg);//复制图像到当前的CvvImage对象中  
	//			cvvImg.DrawToHDC(m_SrcVideoView.GetDC()->GetSafeHdc(), &rect); //绘制图像的ROI区域到DC的rect

	//			pContourImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);


	//			//将原始图像转化为灰阶图像
	//			cvCvtColor(pImg, pContourImg, CV_RGB2GRAY);

	//			cvCanny(pContourImg ,pContourImg, 50, 150, 3);

	//			//灰阶图像的平滑与增强处理
	//			cvSmooth(pContourImg, pContourImg, CV_GAUSSIAN, 5); //高斯滤波

	//			//灰阶图像转化为二值图像
	//			cvThreshold(pContourImg, pContourImg, 50, 150, CV_THRESH_BINARY);

	//			//从二值图像中提取轮廓
	//			int Num = cvFindContours( pContourImg, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);//CV_CHAIN_APPROX_SIMPLE
	//			TRACE("The number of Contours is: %d\n", Num);

	//			for(; contour!=0; contour=contour->h_next)  
	//			{
	//				//获得多边形轮廓
	//				CvSeq *polyContours = cvApproxPoly(contour, sizeof(CvContour), cvCreateMemStorage(0), CV_POLY_APPROX_DP, cvContourPerimeter(contour) * 0.05, 0);

	//				if (polyContours->total == 4)
	//				{
	//					//获得轮廓外框
	//					CvRect boundingRect = cvBoundingRect(polyContours, 0);
	//					//检查轮廓宽高比
	//					double plate_HWratio = ((double) boundingRect.width) / ((double) boundingRect.height);

	//					if ( boundingRect.width > 80 && boundingRect.height > 40)//plate_HWratio > 2.8 && plate_HWratio < 3.4 &&
	//					{

	//						TRACE("polyContours->total = %d, Area = %d\n", polyContours->total, cvContourArea(polyContours, CV_WHOLE_SEQ));
	//						TRACE("Width = %d, Height = %d, HWratio = %.2f\n", boundingRect.width, boundingRect.height, plate_HWratio);
	//						//将轮廓画出
	//						cvDrawContours(pContourImg, polyContours, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
	//					}
	//				} else {
	//					//TRACE("polyContours->total = %d\n", polyContours->total);
	//				}
	//			}

	//			cvvImg.CopyOf(pContourImg);//复制图像到当前的CvvImage对象中  
	//			cvvImg.DrawToHDC(m_ConVideoView.GetDC()->GetSafeHdc(), &rect); //绘制图像的ROI区域到DC的rect


	//			//释放图像
	//			cvReleaseImage( &pImg );
	//			cvReleaseImage( &pContourImg );
	//		}



	//		//CJpegFile *pJpgFile  = new CJpegFile();
	//		//if(pJpgFile)
	//		//{
	//		//	pJpgFile->VertFlipBuf(buf, nWidth*3, nHeight);
	//		//	pJpgFile->BGRFromRGB(buf, nWidth, nHeight);
	//		//	bSize = pJpgFile->RGBToJpegFile(pszFile, buf, nWidth, nHeight, TRUE, 100);

	//		//	delete pJpgFile;
	//		//	pJpgFile = NULL;

	//		//}
	//		delete [] buf;
	//		buf = NULL;
	//	}	
	//}

	//return bSize;
	return 0;
}

void CTestDlg::OnButton43() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		SetAudioPreview(g_hDSP[m_Current], false);
	}	
}

CString CTestDlg::GetBaseDir(const CString &path)
{
	CString out = "";

	int iSlashPos = path.ReverseFind('\\');
	if (iSlashPos !=-1) 
	{
		out = path.Left(iSlashPos);
	} 
	else
	{
		iSlashPos = path.ReverseFind('/');
		if (iSlashPos !=-1) 
		{
			out = path.Left(iSlashPos);
		} 
	}

	return out;
}

void CTestDlg::OnButton44() 
{
	// TODO: Add your control notification handler code here
	StartVideoOutput(0, m_nindex);
	m_nindex++;
}

void CTestDlg::OnButton45() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		CString strText;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strText);

		DWORD dwData = 0x00;
		DRV_RW rw;
		rw.dwType = m_Type.GetCurSel();
		rw.dwAddr = atoi(strText);
		rw.pData = (DWORD)&dwData;
		if(ReadReg(g_hDSP[m_Current], &rw))
		{
			strText.Format("0x%x", dwData);
			SetDlgItemText(IDC_EDIT2, strText);
		}

	}


}

void CTestDlg::OnButton46() 
{
	// TODO: Add your control notification handler code here
	if(g_hDSP[m_Current]!= (HANDLE) 0xffffffff)
	{
		CString strText;
		DWORD dwData = 0x00;
		DRV_RW rw;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strText);

		rw.dwType = m_Type.GetCurSel();
		rw.dwAddr = atoi(strText);
		GetDlgItem(IDC_EDIT2)->GetWindowText(strText);
		dwData =  atoi(strText);
		rw.pData = (DWORD)&dwData;
		//WriteReg(g_hDSP[m_Current], rw);

		GBI2CWrite(g_hDSP[m_Current], rw.dwAddr, dwData );
		dwData+=1;
		strText.Format("%d", dwData);
		GetDlgItem(IDC_EDIT2)->SetWindowText(strText);

	}	
}

void CTestDlg::OnButton47() 
{
	// TODO: Add your control notification handler code here

	m_nCount++;
	DWORD dwMask = 0x01;
	DWORD dwResult = dwMask<<m_nCount;
	dwMask = dwResult;
	GpioGetData(1, &dwResult);
	dwResult|=dwMask;
	GpioSetData(1, dwResult);

	GpioGetData(1, &dwResult);
	CString strText;
	strText.Format(_T("0x%x"), dwResult);
	//	AfxMessageBox(strText);


}


void CTestDlg::OnBnClickedButton36()
{
	// TODO: 在此添加控件通知处理程序代码
	int nBaseNum[4];
	CString sTemp;
	for (int i = 0; i < 4; i++)
	{
		m_edtXH[i].GetWindowText(sTemp);
		nBaseNum[i] = _ttoi(sTemp);
		sTemp.Format(_T("UPDATE CamInfo SET BaseNum = '%d' WHERE ID = %d"), nBaseNum[i], m_nIDs[i]);
		g_DB.ExecuteSQL((_bstr_t)sTemp);
		for (int j = 0; j < g_TotalChannel * 4; j++)
		{
			if (m_pCamInfos[j].nID == m_nIDs[i])
			{
				m_pCamInfos[j].nBaseNum = nBaseNum[i];
			}
		}
	}
}


void CTestDlg::OnBnClickedButton37()
{
	// TODO: 在此添加控件通知处理程序代码
	RECT m_rect;
	CString str;
	m_btnYL.GetWindowText(str);
	if (str == _T("开启预览"))
	{
		for( int i = 0; i < g_TotalChannel; i++)
		{
			if(g_hDSP[i]!= (HANDLE) 0xffffffff)
			{
				m_pVideoView[i]->GetClientRect(&m_rect);

				StartVideoPreview(g_hDSP[i], m_pVideoView[i]->m_hWnd, &m_rect, false, 0, 0);
			}
		}
		m_btnYL.SetWindowText(_T("关闭预览"));
	}else
	{
		for( int i = 0; i < g_TotalChannel; i++)
		{
			if(g_hDSP[i]!= (HANDLE) 0xffffffff)
			{
				m_pVideoView[i]->GetClientRect(&m_rect);

				StopVideoPreview(g_hDSP[i]);
			}
		}
		m_btnYL.SetWindowText(_T("开启预览"));
	}
}


