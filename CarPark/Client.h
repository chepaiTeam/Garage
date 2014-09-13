#include "CSDef.h"


class CClient
{
public:
	CClient();
	~CClient();
protected:

public:
	// ����Client����Ϣ
	void InitClient();

	bool ConectToServer();

	bool CloseSocket();

	/// <summary> 
	/// �㲥������Ϣ�߳�
	/// </summary> 
	static void CCastRecvThread(LPVOID pParam);

	/// <summary> 
	/// �����㲥����
	/// </summary> 
	static void CJXDataThread(LPVOID pParam);

	/// <summary> 
	/// �����ͷ����߳�
	/// </summary> 
	static void StartClientThread(LPVOID pParam);

	/// <summary> 
	/// ������Ϣ���������߳�
	/// </summary> 
	static void CTCPSendThread(LPVOID pParam);

	/// <summary> 
	/// ���շ�������Ϣ�߳�
	/// </summary> 
	static void CTCPRecvThread(LPVOID pParam);
};

