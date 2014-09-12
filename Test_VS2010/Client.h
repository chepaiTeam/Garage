#include "CSDef.h"


class CClient
{
public:
	CClient();
	~CClient();
protected:

public:
	// 监听Client的消息
	void InitClient();

	bool ConectToServer();

	bool CloseSocket();

	/// <summary> 
	/// 广播接收消息线程
	/// </summary> 
	static void CCastRecvThread(LPVOID pParam);

	/// <summary> 
	/// 解析广播数据
	/// </summary> 
	static void CJXDataThread(LPVOID pParam);

	/// <summary> 
	/// 启动客服端线程
	/// </summary> 
	static void StartClientThread(LPVOID pParam);

	/// <summary> 
	/// 发送消息到服务器线程
	/// </summary> 
	static void CTCPSendThread(LPVOID pParam);

	/// <summary> 
	/// 接收服务器消息线程
	/// </summary> 
	static void CTCPRecvThread(LPVOID pParam);
};

