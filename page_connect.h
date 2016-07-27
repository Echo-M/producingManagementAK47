#pragma once

#include "page.h"
#include "resource.h"
#include "label.h"
#include "anchor_layout.h"
#include "thread.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

#define DEF_PACKET_SIZE 32 // ���ݲ��ֵĳ���
#define ECHO_REQUEST 8 // ������ԣ����ͣ�
#define ECHO_REPLY 0 // ����Ӧ�����ͣ�

// IPͷ��
struct IPHeader
{
	BYTE m_byVerHLen; //4λ�汾+4λ�ײ�����
	BYTE m_byTOS; //��������
	USHORT m_usTotalLen; //�ܳ���
	USHORT m_usID; //��ʶ
	USHORT m_usFlagFragOffset; //3λ��־+13λƬƫ��
	BYTE m_byTTL; //TTL
	BYTE m_byProtocol; //Э��
	USHORT m_usHChecksum; //�ײ������
	ULONG m_ulSrcIP; //ԴIP��ַ
	ULONG m_ulDestIP; //Ŀ��IP��ַ
};

// ICMP����ͷ��
struct ICMPHeader
{
	BYTE m_byType;			//��Ϣ����
	BYTE m_byCode;			//����
	USHORT m_usChecksum;	//����� 
	USHORT m_usID;			//��ʶ��
	USHORT m_usSeq;			//���
	ULONG m_ulTimeStamp;	//ʱ������Ǳ�׼ICMPͷ����
};

struct PingReply
{
	USHORT m_usSeq;
	DWORD m_dwRoundTripTime;
	DWORD m_dwBytes;
	DWORD m_dwTTL;
};

class CPing
{
public:
	CPing();
	~CPing();
	BOOL Ping(DWORD dwDestIP, PingReply *pPingReply = NULL, DWORD dwTimeout = 2000);
	BOOL Ping(char *szDestIP, PingReply *pPingReply = NULL, DWORD dwTimeout = 2000);
private:
	BOOL PingCore(DWORD dwDestIP, PingReply *pPingReply, DWORD dwTimeout);
	USHORT CalCheckSum(USHORT *pBuffer, int nSize);
	ULONG GetTickCountCalibrate();
private:
	SOCKET m_sockRaw;			// ԭʼ�׽���
	WSAEVENT m_event;			// 
	USHORT m_usCurrentProcID;	// ����ID
	char *m_szICMPData;			// �����������ICMP���
	BOOL m_bIsInitSucc;			// ��ʼ���ɹ���־
private:
	static USHORT s_usPacketSeq;// ���
};

// PageConnect dialog
class PageConnect : public Page
{
	DECLARE_DYNAMIC(PageConnect)

public:
	PageConnect(CWnd* pParent = NULL);   // standard constructor
	virtual ~PageConnect();

// Dialog Data
	enum { IDD = IDD_PAGE_CONNECT };

  virtual void OnEnter(int leavePageId);
  virtual bool OnLeave(int enterPageId);

protected:
  Label                 labelTips_;
  AnchorLayout          layout_;
  Thread                connectThread_;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  DWORD TryConnect();

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnSize(UINT nType, int cx, int cy);
};
