// stdafx.cpp : source file that includes just the standard includes
//	Test.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CDataBase g_DB;
bool g_bAppRun;
CString g_sHostName;							//������
int g_nNetType;									//�ÿ��Ƴ������������ NETTYPE_CLIENT:�ͻ���  NETTYPE_SERVER��������
CArray<CAM_INFO *, CAM_INFO *&> g_pCamInfos;						//�����Ϣ�ṹ��
CArray<CString> g_CamGroupID;							//���豸���з���

CSerialPort* g_pPort1;							//����1
CSerialPort* g_pPort2;							//����2

BOOL g_bComInit;		//COM���Ƿ��

BOOL g_bDeviceInit;		//�豸�Ƿ��ʼ��