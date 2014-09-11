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

	if (m_pCamInfos)
	{
		delete m_pCamInfos;
		m_pCamInfos = NULL;
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
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonInit)

	ON_BN_CLICKED(IDC_BUTTON2X2, OnButton2x2)
	ON_BN_CLICKED(IDC_BUTTON3X3, OnButton3x3)
	ON_BN_CLICKED(IDC_BUTTON4X4, OnButton4x4)
	ON_BN_CLICKED(IDC_BUTTON6X6, OnButton6x6)
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
	ShowWindow(SW_SHOWMAXIMIZED);

	RegisterMessageNotifyHandle(this->GetSafeHwnd(), 1900);
	TCHAR buf[MAX_PATH+1];
	DWORD res = GetModuleFileName(AfxGetInstanceHandle(), buf, MAX_PATH);
	strDir = GetBaseDir(buf);

	// TODO: Add extra initialization here
	
	for (int i=0; i<36; i++)
	{
		m_pVideoView[i]  = new CVideoView();
		m_pVideoView[i]->Create(_T("Video Static"), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 10, 10), this, ID_VIDEOSTATIC + i);
	}

	ConfigSubChannelSplit(7, 2);

	int i;
	for( i = 0; i<MAX_VIDEO_CHANNEL; i++)
	{
		g_hDSP[i] = (HANDLE)0xffffffff;
	}

	m_cboXD.AddString(_T("ȫ��"));
	m_cboXD.AddString(_T("�ŵ�1"));
	m_cboXD.AddString(_T("�ŵ�2"));
	m_cboXD.AddString(_T("�ŵ�3"));
	m_cboXD.AddString(_T("�ŵ�4"));
	m_cboXD.SetCurSel(0);

	m_nIDs[0] = -1;
	m_nIDs[1] = -1;
	m_nIDs[2] = -1;
	m_nIDs[3] = -1;

	//��������1���ݲɼ�
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
		::MessageBox(this->GetSafeHwnd(), _T("�����豸ʧ�ܣ�������ؽӿڣ�"), _T("����"), MB_ICONERROR|MB_OK);
	}

	InitRect();
	OnButton4x4();

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

	//CvvImage cvvImg; //CvvImage��

	unsigned int uCamID = i + j * 4;
	if(ui->m_pCamInfos[uCamID].nEffective == 0)
	{
		return;
	}

	//���òü�����
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

	//��ԭʼͼ��ת��Ϊ�ҽ�ͼ��
	cvCvtColor(pImgEx, pContourImg, CV_RGB2GRAY);

	cvCanny(pContourImg ,pContourImg, 50, 100, 3);

	//�ҽ�ͼ���ƽ������ǿ����
	cvSmooth(pContourImg, pContourImg, CV_GAUSSIAN, 5); //��˹�˲�

	//�ҽ�ͼ��ת��Ϊ��ֵͼ��
	cvThreshold(pContourImg, pContourImg, 50, 100, CV_THRESH_BINARY);

	CvMemStorage * storage = cvCreateMemStorage(0);  
	CvSeq * contour = NULL;

	//�Ӷ�ֵͼ������ȡ����
	Num = cvFindContours( pContourImg, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);//CV_CHAIN_APPROX_SIMPLE
	//TRACE("The number of Contours is: %d\n", Num);


	//for(; contour!=0; contour=contour->h_next)  
	//{
	//	//��ö��������
	//	CvSeq *polyContours = cvApproxPoly(contour, sizeof(CvContour), cvCreateMemStorage(0), CV_POLY_APPROX_DP, cvContourPerimeter(contour) * 0.05, 0);

	//	if (polyContours->total == 4)
	//	{
	//		//����������
	//		CvRect boundingRect = cvBoundingRect(polyContours, 0);
	//		//���������߱�
	//		double plate_HWratio = ((double) boundingRect.width) / ((double) boundingRect.height);

	//		if ( boundingRect.width > 80 && boundingRect.height > 40)//plate_HWratio > 2.8 && plate_HWratio < 3.4 &&
	//		{

	//			TRACE("polyContours->total = %d, Area = %d\n", polyContours->total, cvContourArea(polyContours, CV_WHOLE_SEQ));
	//			TRACE("Width = %d, Height = %d, HWratio = %.2f\n", boundingRect.width, boundingRect.height, plate_HWratio);
	//			//����������
	//			cvDrawContours(pContourImg, polyContours, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
	//		}
	//	} else {
	//		//TRACE("polyContours->total = %d\n", polyContours->total);
	//	}
	//}



	if (j == ui->m_Current)
	{
		str.Format(_T("����ͷID��%d\t�ź�����%d"), ui->m_pCamInfos[uCamID].nID, Num);
		ui->m_stcXD[i].SetWindowText(str);
		if (ui->m_nIDs[i] != ui->m_pCamInfos[uCamID].nID)
		{
			ui->m_nIDs[i] = ui->m_pCamInfos[uCamID].nID;
			str.Format(_T("%d"), ui->m_pCamInfos[uCamID].nBaseNum);
			ui->m_edtXH[i].SetWindowText(str);
		}

		//cvvImg.CopyOf(pImg);//����ͼ�񵽵�ǰ��CvvImage������  
		//cvvImg.DrawToHDC(ui->m_SrcVideoView.GetDC()->GetSafeHdc(), &rect);
		//cvvImg.Destroy();

		//cvvImg.CopyOf(pContourImg);//����ͼ�񵽵�ǰ��CvvImage������  
		//cvvImg.DrawToHDC(ui->m_ConVideoView.GetDC()->GetSafeHdc(), &rect);
		//cvvImg.Destroy();
	}

	//����̶�ֵ
	if (Num > ui->m_pCamInfos[uCamID].nBaseNum)
	{
		ui->m_pCamInfos[uCamID].nIsCar = 1;
	}else
	{
		ui->m_pCamInfos[uCamID].nIsCar = 0;
	}

	////����ƫ����
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

	//�ͷ�ͼ��
	cvReleaseImage( &pImg );
	cvReleaseImage( &pImgEx );
	cvReleaseImage( &pContourImg );

	delete[] buf;
	buf = NULL;

	delete pCvInfo;
	pCvInfo = NULL;
}

void CTestDlg::OnButton2() 
{
	int i;

	for( i = 0;i<g_TotalChannel;i++)
	{
		StartDsp(g_hDSP[i]);
		Set_Main_Enable(g_hDSP[i],0);
	}

	for( i = 0;i<g_TotalChannel;i++)
	{
		StartDMA(g_hDSP[i]);
	}

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
			//start=clock();//������ʼʱ
			//finish=clock();//��������ʱ
			//duration=finish-start;
			//CString aa;
			//aa.Format("ִ��ʱ��  %d",duration);
			//AfxMessageBox(aa);

		//���㳵������
		uCarNum = 0;
		for (i = 0; i < uCamNum; i++)
		{
			if(ui->m_pCamInfos[i].nIsCar == 1)
				uCarNum++;
		}

		unsigned int uSYCW = ui->m_LedInfo.uParkingLotNum < uCarNum ? 0 : (ui->m_LedInfo.uParkingLotNum - uCarNum);

		//�������������ı���ɫ
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

		//�������������ı���ɫ
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


void CTestDlg::SetParame(int nChannel)
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
			SetVideoStandard(g_hDSP[nChannel],StandardPAL);
		}
		else
		{
			SetVideoStandard(g_hDSP[nChannel],StandardNTSC);
		}
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

		int nPos = m_Slider_FrameRate.GetPos();
		if(nChannel ==0)
		{
			//nPos = 12;
		}		
		SetFrameRate(nPos);

		SetOsd(g_hDSP[nChannel],true);



		int nBr = m_Slider_BR.GetPos();	
		int nCo = m_Slider_CO.GetPos();	
		int nSa = m_Slider_SA.GetPos();	
		int nHu = m_Slider_HU.GetPos();	
		SetVideoPara(g_hDSP[nChannel],nBr,nCo,nSa,nHu);	

		int nSH = m_Slider_SH.GetPos();	
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

void CTestDlg::OnButton3() 
{
	//StopAllDSPs();
	for(int i = 0;i<g_TotalChannel;i++)
	{
		SetParame(i);
	}
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

		//����ṹ�ĸ���
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
			//��Ĭ��ֵ
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
	m_stcXD[0].SetWindowText(_T("����ͷID��0\t�ź�����0"));
	m_stcXD[1].MoveWindow(m_Onerect.right + 50, 130, 160, 25);
	m_stcXD[1].SetWindowText(_T("����ͷID��0\t�ź�����0"));
	m_stcXD[2].MoveWindow(m_Onerect.right + 50, 170, 160, 25);
	m_stcXD[2].SetWindowText(_T("����ͷID��0\t�ź�����0"));
	m_stcXD[3].MoveWindow(m_Onerect.right + 50, 210, 160, 25);
	m_stcXD[3].SetWindowText(_T("����ͷID��0\t�ź�����0"));

	m_cboXD.MoveWindow(m_Onerect.right + 50, 250, 120, 125);

	m_edtXH[0].MoveWindow(m_Onerect.right + 210, 90, 80, 25);
	m_edtXH[1].MoveWindow(m_Onerect.right + 210, 130, 80, 25);
	m_edtXH[2].MoveWindow(m_Onerect.right + 210, 170, 80, 25);
	m_edtXH[3].MoveWindow(m_Onerect.right + 210, 210, 80, 25);

	m_btnBC.MoveWindow(m_Onerect.right + 210, 250, 80, 25);
	m_btnBC.SetWindowTextA(_T("����"));

	m_btnYL.MoveWindow(m_Onerect.right + 50, 290, 120, 125);
	m_btnYL.SetWindowText(_T("����Ԥ��"));
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


void CTestDlg::OnBnClickedButton36()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RECT m_rect;
	CString str;
	m_btnYL.GetWindowText(str);
	if (str == _T("����Ԥ��"))
	{
		for( int i = 0; i < g_TotalChannel; i++)
		{
			if(g_hDSP[i]!= (HANDLE) 0xffffffff)
			{
				m_pVideoView[i]->GetClientRect(&m_rect);

				StartVideoPreview(g_hDSP[i], m_pVideoView[i]->m_hWnd, &m_rect, false, 0, 0);
			}
		}
		m_btnYL.SetWindowText(_T("�ر�Ԥ��"));
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
		m_btnYL.SetWindowText(_T("����Ԥ��"));
	}
}


