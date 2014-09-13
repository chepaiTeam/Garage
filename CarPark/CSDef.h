#pragma once

#define MAX_PACKET_SIZE			10240    // ���ݰ�����󳤶�,��λ��sizeof(char)
#define MAXFILEDIRLENGTH		256     // ����ļ�·������󳤶�
#define SERVER_PORT_CAST		4096    // �������˿ں�
#define SERVER_PORT_TCP			4097    // �������˿ں�
#define CAST_PORT				4098    // �㲥�˿ں�
#define CLIENT_PORT_TCP			4099    // �ͻ��˶˿ں�


// ������Ϣ�ĺ궨��
#define INVALID_MSG					-1		// ��Ч����Ϣ��ʶ
#define MSG_DEVICEINFO				 1		// PC�豸��Ϣ
#define MSG_LEDINFO					 2		// LED�豸��Ϣ
#define MSG_OPENFILE_ERROR			 10		// ������Ϣ
#define MSG_FILEALREADYEXIT_ERROR	 11		// ������Ϣ

class CCSDef
{
public:
#pragma pack(1)      // ʹ�ṹ������ݰ���1�ֽ�������,ʡ�ռ�

	// ��Ϣͷ
	struct TMSG_HEADER
	{
		char    cMsgID;    // ��Ϣ��ʶ

		TMSG_HEADER(char MsgID = INVALID_MSG)
			: cMsgID(MsgID)
		{
		}
	};

	// �ɿͻ��˴��͸��������ĵ�ǰ�豸��Ϣ
	struct TMSG_DEVICEINFO : public TMSG_HEADER
	{
		char szDeviceName[256];		// �豸��
		int nRedLightNum;			// �����
		int nGreenLightNum;			// �̵���
		int nNoneLightNum;			// δʹ�õ���

		TMSG_DEVICEINFO()
			: TMSG_HEADER(MSG_DEVICEINFO)
		{
		}
	};

	// �ɷ��������͸��ͻ��˵ĵ�ǰLED��Ϣ
	struct TMSG_LEDINFO : public TMSG_HEADER
	{
		char szBuff[MAX_PACKET_SIZE];

		TMSG_LEDINFO()
			: TMSG_HEADER(MSG_LEDINFO)
		{
		}
	};


	// ���ͳ�����Ϣ,����:
	struct TMSG_ERROR_MSG : public TMSG_HEADER
	{
		TMSG_ERROR_MSG(char cErrorMsg)
			: TMSG_HEADER(cErrorMsg)
		{
		}
	};

	//// �����ļ�
	//struct TMSG_FILE : public TMSG_HEADER
	//{
	//	union     // ����union��֤�����ݰ��Ĵ�С������MAX_PACKET_SIZE * sizeof(char)
	//	{
	//		char szBuff[MAX_PACKET_SIZE];
	//		struct
	//		{
	//			int nStart;
	//			int nSize;
	//			char szBuff[MAX_PACKET_SIZE - 2 * sizeof(int)];
	//		}tFile;
	//	};

	//	TMSG_FILE()
	//		: TMSG_HEADER(MSG_FILE)
	//	{

	//	}
	//};

#pragma pack()
};