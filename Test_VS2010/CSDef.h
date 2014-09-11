#pragma once

#define MAX_PACKET_SIZE   10240    // ���ݰ�����󳤶�,��λ��sizeof(char)
#define MAXFILEDIRLENGTH 256     // ����ļ�·������󳤶�
#define PORT     4096    // �˿ں�
#define SERVER_IP    "127.0.0.1" // server�˵�IP��ַ

// ������Ϣ�ĺ궨��
#define INVALID_MSG      -1   // ��Ч����Ϣ��ʶ
#define MSG_FILENAME     1   // �ļ�������
#define MSG_FILELENGTH     2   // �����ļ��ĳ���
#define MSG_CLIENT_READY    3   // �ͻ���׼�������ļ�
#define MSG_FILE      4   // �����ļ�
#define MSG_SENDFILESUCCESS    5   // �����ļ��ɹ�
#define MSG_OPENFILE_ERROR    10   // ���ļ�ʧ��,�������ļ�·�������Ҳ����ļ���ԭ��
#define MSG_FILEALREADYEXIT_ERROR 11   // Ҫ������ļ��Ѿ�������

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

	// �����͵��ļ���
	// �ͻ��˴����������˵���ȫ·������
	// ���������ظ��ͻ��˵����ļ���
	struct TMSG_FILENAME : public TMSG_HEADER
	{
		char szFileName[256];   // �����ļ������ַ�����

		TMSG_FILENAME()
			: TMSG_HEADER(MSG_FILENAME)
		{
		}
	};

	// �����ļ�����
	struct TMSG_FILELENGTH : public TMSG_HEADER
	{
		long lLength;

		TMSG_FILELENGTH(long length)
			: TMSG_HEADER(MSG_FILELENGTH), lLength(length) 
		{

		}
	};

	// Client���Ѿ�׼������,Ҫ��Server�˿�ʼ�����ļ�
	struct TMSG_CLIENT_READY : public TMSG_HEADER
	{
		TMSG_CLIENT_READY()
			: TMSG_HEADER(MSG_CLIENT_READY)
		{
		}
	};

	// �����ļ�
	struct TMSG_FILE : public TMSG_HEADER
	{
		union     // ����union��֤�����ݰ��Ĵ�С������MAX_PACKET_SIZE * sizeof(char)
		{
			char szBuff[MAX_PACKET_SIZE];
			struct
			{
				int nStart;
				int nSize;
				char szBuff[MAX_PACKET_SIZE - 2 * sizeof(int)];
			}tFile;
		};

		TMSG_FILE()
			: TMSG_HEADER(MSG_FILE)
		{

		}
	};

	// �����ļ��ɹ�
	struct TMSG_SENDFILESUCCESS : public TMSG_HEADER
	{
		TMSG_SENDFILESUCCESS()
			: TMSG_HEADER(MSG_SENDFILESUCCESS)
		{
		}
	};

	// ���ͳ�����Ϣ,����:
	// MSG_OPENFILE_ERROR:���ļ�ʧ��
	// MSG_FILEALREADYEXIT_ERROR:Ҫ������ļ��Ѿ�������
	struct TMSG_ERROR_MSG : public TMSG_HEADER
	{
		TMSG_ERROR_MSG(char cErrorMsg)
			: TMSG_HEADER(cErrorMsg)
		{
		}
	};

#pragma pack()
};