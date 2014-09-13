
// CarParkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarPark.h"
#include "CarParkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarParkDlg 对话框

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define ID_VIDEOSTATIC 9001

typedef struct
{
	int i;
	int j;
	BYTE *TempBuffer;
	CCarParkDlg *ui;	//
} CV_INFO, *pCV_INFO;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
int g_TotalDSPs = 0;
int g_TotalChannel = 0;
HANDLE g_hDSP[MAX_VIDEO_CHANNEL];




CCarParkDlg::CCarParkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCarParkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i = 0; i < MAX_VIDEO_CHANNEL; i++)
	{
		g_hDSP[i] = (HANDLE)0xffffffff;
		m_pVideoView[i] = NULL;
	}

	m_bDeviceInit = FALSE;
	m_nViewMode = 4;
	g_pPort1 = NULL;
	g_pPort2 = NULL;
	m_pServer = NULL;
	m_pClient = NULL;
}

CCarParkDlg::~CCarParkDlg()
{
	for (int i = 0; i < MAX_VIDEO_CHANNEL; i++)
	{
		if (m_pVideoView[i])
		{
			delete m_pVideoView[i];
			m_pVideoView[i] = NULL;
		}
	}

	for (int i = 0; i < g_pCamInfos.GetCount(); i++)
	{
		delete g_pCamInfos.GetAt(i);
	}
	g_pCamInfos.RemoveAll();
}

void CCarParkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarParkDlg)
	DDX_Control(pDX, IDC_STC_XD1, m_stcXD[0]);
	DDX_Control(pDX, IDC_STC_XD2, m_stcXD[1]);
	DDX_Control(pDX, IDC_STC_XD3, m_stcXD[2]);
	DDX_Control(pDX, IDC_STC_XD4, m_stcXD[3]);
	DDX_Control(pDX, IDC_CBOXD, m_cboXD);
	DDX_Control(pDX, IDC_EDIT1, m_edtXH[0]);
	DDX_Control(pDX, IDC_EDIT2, m_edtXH[1]);
	DDX_Control(pDX, IDC_EDIT3, m_edtXH[2]);
	DDX_Control(pDX, IDC_EDIT4, m_edtXH[3]);
	DDX_Control(pDX, IDC_BUTTONSAVE, m_btnBC);
	DDX_Control(pDX, IDC_BUTTONOPEN, m_btnYL);
	DDX_Control(pDX, IDC_STATIC_NET, m_stcNet);
	DDX_Control(pDX, IDC_RADIO1, m_rdoClient);
	DDX_Control(pDX, IDC_RADIO2, m_rdoServer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCarParkDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2X2, OnButton2x2)
	ON_BN_CLICKED(IDC_BUTTON3X3, OnButton3x3)
	ON_BN_CLICKED(IDC_BUTTON4X4, OnButton4x4)
	ON_BN_CLICKED(IDC_BUTTON6X6, OnButton6x6)
	ON_BN_CLICKED(IDC_BUTTONSAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTONOPEN, OnBnClickedButtonOpenView)

	ON_MESSAGE(WM_VIEW_CLICK, OnMsgViewClick)
	ON_MESSAGE(WM_VIEW_DBCLICK, OnMsgViewDbClick)
END_MESSAGE_MAP()


// CCarParkDlg 消息处理程序

BOOL CCarParkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//ShowWindow(SW_SHOWMAXIMIZED);
	CString sTitle;
	sTitle.Format(_T("设备号：%s"), g_sDeviceID);
	SetWindowText(sTitle);

	RegisterMessageNotifyHandle(this->GetSafeHwnd(), 1900);
	TCHAR buf[MAX_PATH+1];
	DWORD res = GetModuleFileName(AfxGetInstanceHandle(), buf, MAX_PATH);
	strDir = GetBaseDir(buf);

	// TODO: Add extra initialization here
	for (int i = 0; i < MAX_VIDEO_CHANNEL; i++)
	{
		m_pVideoView[i] = new CVideoView();
		m_pVideoView[i]->Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + i);
	}

	for(int i = 0;i<MAX_VIDEO_CHANNEL;i++)
	{
		g_hDSP[i] = (HANDLE)0xffffffff;
	}

	ConfigSubChannelSplit(7, 2);

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

	InitRect();

	//启动串口1数据采集
	g_pPort1 = new CSerialPort(); 
	g_pPort2 = new CSerialPort(); 

	if (g_pPort1->Open(_T("COM1"), 9600U, 'N', 8U, 1U, EV_RXCHAR, 1024U)
		&& g_pPort2->Open(_T("COM2"), 9600U, 'N', 8U, 1U, EV_RXCHAR, 1024U))
	{
		g_pPort1->StartMonitoring();
		g_pPort1->WriteToPort(_T("@1A1&"));
		//m_pPort->WriteToPort(_T("@1B000&"));

		g_pPort2->StartMonitoring();
		g_pPort2->WriteToPort(_T("@1A1&"));

		//采集线程
		::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CamThread, this, 0, NULL);
	}else{
		MessageBox(_T("连接设备失败，请检查相关接口！"), _T("警告"), MB_ICONERROR|MB_OK);
	}

	if (g_nNetType == NETTYPE_CLIENT)
	{
		m_rdoClient.SetCheck(TRUE);
	}else
	{
		m_rdoServer.SetCheck(TRUE);
		m_pServer = new CServer();
		m_pServer->InitServer();
	}

	m_pClient = new CClient;
	m_pClient->InitClient();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCarParkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCarParkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCarParkDlg::CamThread(LPVOID pParam)
{
	CCarParkDlg *ui = (CCarParkDlg*)pParam;

	//CWnd *bt = ui->GetDlgItem(IDC_BUTTON42);

	CString str;

	int i, j;//, ch = 1, grop = 1;

	ULONG size=1024*1024;

	CRect rect;
	int result;

	//ui->m_SrcVideoView.GetClientRect(&rect);

	ui->DeviceInit();
	Sleep(100);
	ui->StartDevice();
	Sleep(500);

	unsigned int uCamNum = g_TotalChannel * 4;
	unsigned int uCarNum = 0;

	int nXDSel;
	int nBinXD = 0, nEndXD = 0;

	while(g_bAppRun)
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

		for (i = nBinXD; (i < nEndXD) && g_bAppRun; i++)
		{
			str.Format(_T("@1A%d&"), i + 1);
			g_pPort1->WriteToPort(str);

			str.Format(_T("@1A%d&"), i + 1);
			g_pPort2->WriteToPort(str);
			Sleep(700);

			for (j = 0; (j < g_TotalChannel) && g_bAppRun; j++)
			{	
				if(g_hDSP[j] != (HANDLE)0xffffffff)
				{
					//分配通道缓存数据
					BYTE *TempBuf = new BYTE[1024 * 1024];

					result = GetOriginalImage(g_hDSP[j], TempBuf, &size);

					CV_INFO *pCvInfo = new CV_INFO;
					pCvInfo->i = i;
					pCvInfo->j = j;
					pCvInfo->ui = ui;
					pCvInfo->TempBuffer = TempBuf;
					if(result == 0)
					{
						::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CVData, pCvInfo, 0, NULL);
					}else
					{
						delete[] TempBuf;
						TempBuf = NULL;

						delete pCvInfo;
						pCvInfo = NULL;
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
	}
}

void CCarParkDlg::CVData(LPVOID pParam)
{
	CV_INFO *pCvInfo = (CV_INFO*)pParam;
	CCarParkDlg *ui = pCvInfo->ui;
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
	if(g_pCamInfos[uCamID]->nLightColor == 0)
	{
		return;
	}

	//设置裁剪区域
	ROIRect.x = g_pCamInfos[uCamID]->nRectX;
	ROIRect.y = g_pCamInfos[uCamID]->nRectY;
	ROIRect.width = g_pCamInfos[uCamID]->nRectW;
	ROIRect.height = g_pCamInfos[uCamID]->nRectH;

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
	cvCopy(pImg, pImgEx);
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
		str.Format(_T("摄像头ID：%d\t信号量：%d"), g_pCamInfos[uCamID]->nID, Num);
		ui->m_stcXD[i].SetWindowText(str);
		if (ui->m_nIDs[i] != g_pCamInfos[uCamID]->nID)
		{
			ui->m_nIDs[i] = g_pCamInfos[uCamID]->nID;
			str.Format(_T("%d"), g_pCamInfos[uCamID]->nBaseNum);
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
	if (Num > g_pCamInfos[uCamID]->nBaseNum)
	{
		g_pCamInfos[uCamID]->nLightColor = 1;
	}else
	{
		g_pCamInfos[uCamID]->nLightColor = 2;
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
		g_pCamInfos[uCamID]->nLightColor);
	if (j > 7)
	{
		g_pPort2->WriteToPort(str);
	}else
	{
		g_pPort1->WriteToPort(str);
	}

	cvReleaseMemStorage( &storage );

	//释放图像
	cvReleaseImage( &pImg );
	cvReleaseImage( &pImgEx );
	cvReleaseImage( &pContourImg );

	delete[] buf;
	buf = NULL;

	delete[] pCvInfo->TempBuffer;
	pCvInfo->TempBuffer = NULL;

	delete pCvInfo;
	pCvInfo = NULL;
}

void CCarParkDlg::DeviceInit() 
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

	//设置摄像头参数
	ParameInit();

	if(g_hDSP[m_Current] != (HANDLE) 0xffffffff)
	{
		TCHAR szFile[256];
		__stprintf(szFile, _T("%s//bk.dll"), strDir);
		SetDefaultDisplayPicture(g_hDSP[m_Current], szFile);
	}
	SYSTEM_BOARD_INFO system_info;
	GetSystemBoardInfo(&system_info);
	GpioInit(1, 0xffffff00);

	LoadCamData();

	m_bDeviceInit = TRUE;
}

void CCarParkDlg::StartDevice() 
{
	int i;
	for( i = 0; i < g_TotalChannel; i++)
	{
		StartDsp(g_hDSP[i]);
		Set_Main_Enable(g_hDSP[i],0);
	}

	for( i = 0; i < g_TotalChannel; i++)
	{
		StartDMA(g_hDSP[i]);
	}
}

void CCarParkDlg::ParameInit()
{
	//StopAllDSPs();
	for(int nChannel = 0; nChannel < g_TotalChannel; nChannel++)
	{
		if(g_hDSP[nChannel]!= (HANDLE) 0xffffffff)
		{
			int standard = 0;
			if(standard == 0)
			{
				SetVideoStandard(g_hDSP[nChannel],StandardPAL);
			}
			else
			{
				SetVideoStandard(g_hDSP[nChannel],StandardNTSC);
			}

			int Size = 2;
			switch(Size)
			{
			case 0:
				SetEncoderPictureFormat(g_hDSP[nChannel],ENC_QCIF_FORMAT);
				break;
			case 1:
				SetEncoderPictureFormat(g_hDSP[nChannel],ENC_DCIF_FORMAT);
				break;
			case 2:
				SetEncoderPictureFormat(g_hDSP[nChannel],ENC_CIF_FORMAT);
				break;
			case 3:
				SetEncoderPictureFormat(g_hDSP[nChannel],ENC_SVGA_FORMAT);
				break;
			case 4:
				SetEncoderPictureFormat(g_hDSP[nChannel],ENC_2CIF_FORMAT);
				break;

			default:
				SetEncoderPictureFormat(g_hDSP[nChannel],ENC_CIF_FORMAT);
				break;

			}

			int nFrameRate = 12;
			//设置帧频
			SetVideoFrameRate(g_hDSP[nChannel], nFrameRate);

			SetOsd(g_hDSP[nChannel],false);

			int nBr = 100;	
			int nCo = 100;	
			int nSa = 100;	
			int nHu = 100;	
			SetVideoPara(g_hDSP[nChannel], nBr, nCo, nSa, nHu);	

			int nSH = 4;	
			SetVideoSharp(g_hDSP[nChannel],nSH);

			char szChannelName[64];
			sprintf(szChannelName,"Channel  %02d",nChannel+1);
			USHORT		Buf1[100];

			USHORT		Buf2[100]={10,20,_OSD_YEAR4,'-',_OSD_MONTH2,'-',_OSD_DAY,' ',\
				_OSD_HOUR24,':',_OSD_MINUTE,':',_OSD_SECOND,' ','\0'};
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

			WCHAR wszOsdSting[256];          // Unicode user name
			int nCount = MultiByteToWideChar( CP_ACP, 0, szChannelName,
				Length, wszOsdSting,   
				sizeof(wszOsdSting)/sizeof(wszOsdSting[0]) );

			offset=2;
			for (int temp=0; temp<nCount;)
			{
				Buf1[offset]=wszOsdSting[temp];
				offset++;
				temp++;
			}
			Buf1[offset]='\0';
			if(nChannel ==0)
			{
				SetStreamType(g_hDSP[nChannel],STREAM_TYPE_AVSYNC);
			}			
			result=SetOsdDisplayMode(g_hDSP[nChannel],255,FALSE,0x1000,Buf1,Buf2);
		}
	}
}

void CCarParkDlg::LoadCamData()
{
	if (g_TotalChannel > 0)
	{
		int nCamSum = g_TotalChannel * 4;
		int nIndex = 0;

		_RecordsetPtr rs=g_DB.GetRecordset(_T("SELECT * FROM CamInfo"));
		while(!rs->adoEOF && nIndex < nCamSum)
		{
			CAM_INFO *pCamInfo = new CAM_INFO;
			if (rs->GetCollect(_T("ID")).vt != VT_NULL)
			{
				pCamInfo->nID = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("ID")));
			}

			if (rs->GetCollect(_T("LightColor")).vt != VT_NULL)
			{
				pCamInfo->nLightColor = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("LightColor")));
			}

			if (rs->GetCollect(_T("BaseNum")).vt != VT_NULL)
			{
				pCamInfo->nBaseNum = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("BaseNum")));
			}

			if (rs->GetCollect(_T("Offset")).vt != VT_NULL)
			{
				pCamInfo->nOffset = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("Offset")));
			}

			if (rs->GetCollect(_T("RectX")).vt != VT_NULL)
			{
				pCamInfo->nRectX = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectX")));
			}

			if (rs->GetCollect(_T("RectY")).vt != VT_NULL)
			{
				pCamInfo->nRectY = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectY")));
			}

			if (rs->GetCollect(_T("RectW")).vt != VT_NULL)
			{
				pCamInfo->nRectW = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectW")));
			}

			if (rs->GetCollect(_T("RectH")).vt != VT_NULL)
			{
				pCamInfo->nRectH = _ttoi((LPCSTR)(_bstr_t)rs->GetCollect(_T("RectH")));
			}

			rs->MoveNext();
			nIndex++;

			g_pCamInfos.Add(pCamInfo);
		}
		rs->Close();

		while (nIndex < nCamSum)
		{
			CAM_INFO *pCamInfo = new CAM_INFO;
			//给默认值
			pCamInfo->nID = -1;
			pCamInfo->nLightColor = 1;
			pCamInfo->nBaseNum = 50;
			pCamInfo->nOffset = 10;
			pCamInfo->nRectX = 60;
			pCamInfo->nRectY = 0;
			pCamInfo->nRectW = 200;
			pCamInfo->nRectH = 240;

			nIndex++;
			g_pCamInfos.Add(pCamInfo);
		}
	}
}

LRESULT CCarParkDlg::OnMsgViewDbClick(WPARAM wParam, LPARAM lParam)
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

	for( i = 0; i < g_TotalChannel; i++)
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

LRESULT CCarParkDlg::OnMsgViewClick(WPARAM wParam, LPARAM lParam)
{
	m_Current = wParam;
	return 0;
}

#define VIDEO_CLEARANCE 1
void CCarParkDlg::ShowHideScreen()
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

void CCarParkDlg::OnButton2x2() 
{
	m_nViewMode = 2;
	ShowHideScreen();
}

void CCarParkDlg::OnButton3x3() 
{
	m_nViewMode = 3;
	ShowHideScreen();
}

void CCarParkDlg::OnButton4x4() 
{
	m_nViewMode = 4;
	ShowHideScreen();
}

void CCarParkDlg::OnButton6x6() 
{
	m_nViewMode = 6;
	ShowHideScreen();
}

CString CCarParkDlg::GetBaseDir(const CString &path)
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

void CCarParkDlg::OnBnClickedButtonSave()
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
			if (g_pCamInfos[j]->nID == m_nIDs[i])
			{
				g_pCamInfos[j]->nBaseNum = nBaseNum[i];
			}
		}
	}
}

void CCarParkDlg::OnBnClickedButtonOpenView()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bDeviceInit)
	{
		return;
	}

	RECT m_rect;
	CString str;
	m_btnYL.GetWindowText(str);

	if (str == _T("开启预览"))
	{
		for( int i = 0; i < g_TotalChannel; i++)
		{
			if(g_hDSP[i] != (HANDLE) 0xffffffff)
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
			if(g_hDSP[i] != (HANDLE) 0xffffffff)
			{
				m_pVideoView[i]->GetClientRect(&m_rect);

				StopVideoPreview(g_hDSP[i]);
			}
		}
		m_btnYL.SetWindowText(_T("开启预览"));
	}
}

void CCarParkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pVideoView[0])
		InitRect();
}

void CCarParkDlg::InitRect()
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

	int nToolWidth = rect.right - m_Onerect.right;

	ShowHideScreen();

	//m_SrcVideoView.Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + 100);
	//m_ConVideoView.Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + 100);

	this->GetDlgItem(IDC_BUTTON2X2)->MoveWindow(m_Onerect.right + 10, 10, (nToolWidth - 20) / 4, 50);
	this->GetDlgItem(IDC_BUTTON3X3)->MoveWindow(m_Onerect.right + 10 + (nToolWidth - 20) / 4, 10, (nToolWidth - 20) / 4, 50);
	this->GetDlgItem(IDC_BUTTON4X4)->MoveWindow(m_Onerect.right + 10 + (nToolWidth - 20) / 4 * 2, 10, (nToolWidth - 20) / 4, 50);
	this->GetDlgItem(IDC_BUTTON6X6)->MoveWindow(m_Onerect.right + 10 + (nToolWidth - 20) / 4 * 3, 10, (nToolWidth - 20) / 4, 50);

	//m_SrcVideoView.MoveWindow(m_Onerect.right + 10,  130, rect.right - m_Onerect.right - 20, (rect.right - m_Onerect.right - 20) * 0.75);
	//CRect rectS;
	//m_SrcVideoView.GetWindowRect(rectS);
	//rectS.OffsetRect(0, rectS.Height() + 10);

	//m_ConVideoView.MoveWindow(rectS);

	//this->GetDlgItem(IDC_BUTTON42)->MoveWindow(m_Onerect.right + 10, 70, rect.right - m_Onerect.right - 20, 50);

	m_stcXD[0].MoveWindow(m_Onerect.right + 10, 90, 160, 25);
	m_stcXD[0].SetWindowText(_T("摄像头ID：0\t信号量：0"));
	m_stcXD[1].MoveWindow(m_Onerect.right + 10, 130, 160, 25);
	m_stcXD[1].SetWindowText(_T("摄像头ID：0\t信号量：0"));
	m_stcXD[2].MoveWindow(m_Onerect.right + 10, 170, 160, 25);
	m_stcXD[2].SetWindowText(_T("摄像头ID：0\t信号量：0"));
	m_stcXD[3].MoveWindow(m_Onerect.right + 10, 210, 160, 25);
	m_stcXD[3].SetWindowText(_T("摄像头ID：0\t信号量：0"));

	m_cboXD.MoveWindow(m_Onerect.right + 10, 250, 120, 125);

	m_edtXH[0].MoveWindow(m_Onerect.right + 170, 90, nToolWidth - 180, 25);
	m_edtXH[1].MoveWindow(m_Onerect.right + 170, 130, nToolWidth - 180, 25);
	m_edtXH[2].MoveWindow(m_Onerect.right + 170, 170, nToolWidth - 180, 25);
	m_edtXH[3].MoveWindow(m_Onerect.right + 170, 210, nToolWidth - 180, 25);

	m_btnBC.MoveWindow(m_Onerect.right + 170, 250, nToolWidth - 180, 25);

	m_btnYL.MoveWindow(m_Onerect.right + 10, 300, 120, 25);
	m_btnYL.SetWindowText(_T("开启预览"));

	m_stcNet.MoveWindow(m_Onerect.right + 10, 350, nToolWidth - 20, 80);
	m_rdoClient.MoveWindow(m_Onerect.right + 40, 370, 70, 25);
	m_rdoServer.MoveWindow(m_Onerect.right + 150, 370, 70, 25);
}


BOOL CCarParkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message >= WM_KEYFIRST && pMsg->wParam <= WM_KEYLAST) 
	{ 
		if(pMsg->wParam == VK_RETURN || VK_ESCAPE ==pMsg->wParam)
		{
			return TRUE;
		}
	} 
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CCarParkDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	g_bAppRun = false;
	
	for( int i = 0;i<g_TotalChannel;i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopVideoCapture(g_hDSP[i]);
		}
	}
	for(int i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			StopDsp(g_hDSP[i]);
		}
	}
	Sleep(1);
	for(int i = 0; i<g_TotalChannel; i++)
	{
		if(g_hDSP[i]!= (HANDLE) 0xffffffff)
		{
			ChannelClose(g_hDSP[i]);
		}
	}

	if (g_pPort1)
	{
		g_pPort1->StopMonitoring();
		delete g_pPort1;
	}

	if (g_pPort2)
	{
		g_pPort2->StopMonitoring();
		delete g_pPort2;
	}

	if (m_pServer)
	{
		m_pServer->CloseSocket();
		Sleep(1000);
		delete m_pServer;
		m_pServer = NULL;
	}

	if (m_pClient)
	{
		m_pClient->CloseSocket();
		Sleep(1000);
		delete m_pClient;
		m_pClient = NULL;
	}

	DeInitDSPs();
	return CDialogEx::DestroyWindow();
}

