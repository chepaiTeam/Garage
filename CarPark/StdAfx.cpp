// stdafx.cpp : source file that includes just the standard includes
//	Test.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CDataBase g_DB;
bool g_bAppRun;
CString g_sDeviceID;							//�豸���
int g_nNetType;									//�ÿ��Ƴ������������ NETTYPE_CLIENT:�ͻ���  NETTYPE_SERVER��������
CArray<CAM_INFO *, CAM_INFO *&> g_pCamInfos;							//�����Ϣ�ṹ��

CSerialPort* g_pPort1;							//����1
CSerialPort* g_pPort2;							//����2