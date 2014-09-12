#pragma once
#include "CSDef.h"

typedef struct
{
	int nMin;		//����������С
	int nMax;		//���������޴�
	CString sRgb;	//��ɫֵ
} RGB_INFO, *pRGB_INFO;

typedef struct
{
	CString sAddress;				//��ַ��+��λ
	CString sDirection;					//����
	unsigned int uParkingLotNum;		//��λ����
	CString sTextRgb;					//������ɫ�ַ���
	CString sTagRgb;					//������ɫ�ַ���
	CArray<RGB_INFO*, RGB_INFO*&> TextRgbs;			//������ɫ��Χ
	CArray<RGB_INFO*, RGB_INFO*&> TagRgbs;			//������ɫ��Χ
	int nComNum;					//����λ��
} LED_INFO, *pLED_INFO;

class CServer
{
public:
	CServer(void);
	~CServer(void);
protected:

public:
	/// <summary> 
	/// �������ݿ���LED�豸������Ϣ
	/// </summary> 
	void LoadLedData();

	// �ر�socket��
	bool CloseSocket();
	// ��server������
	bool InitServer();

	/// <summary> 
	/// ������Ϣ�߳�
	/// </summary> 
	static void BroadcastThread(LPVOID pParam);


	/// <summary> 
	/// �����������߳�
	/// </summary> 
	static void StartServerThread(LPVOID pParam);

	/// <summary> 
	/// ������Ϣ�߳�
	/// </summary> 
	static void STCPRecvThread(LPVOID pParam);
};

