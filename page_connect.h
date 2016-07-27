#pragma once

#include "page.h"
#include "resource.h"
#include "label.h"
#include "anchor_layout.h"
#include "thread.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib

#define DEF_PACKET_SIZE 32 // 数据部分的长度
#define ECHO_REQUEST 8 // 请求回显（类型）
#define ECHO_REPLY 0 // 回显应答（类型）

// IP头部
struct IPHeader
{
	BYTE m_byVerHLen; //4位版本+4位首部长度
	BYTE m_byTOS; //服务类型
	USHORT m_usTotalLen; //总长度
	USHORT m_usID; //标识
	USHORT m_usFlagFragOffset; //3位标志+13位片偏移
	BYTE m_byTTL; //TTL
	BYTE m_byProtocol; //协议
	USHORT m_usHChecksum; //首部检验和
	ULONG m_ulSrcIP; //源IP地址
	ULONG m_ulDestIP; //目的IP地址
};

// ICMP报文头部
struct ICMPHeader
{
	BYTE m_byType;			//消息类型
	BYTE m_byCode;			//代码
	USHORT m_usChecksum;	//检验和 
	USHORT m_usID;			//标识符
	USHORT m_usSeq;			//序号
	ULONG m_ulTimeStamp;	//时间戳（非标准ICMP头部）
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
	SOCKET m_sockRaw;			// 原始套接字
	WSAEVENT m_event;			// 
	USHORT m_usCurrentProcID;	// 进程ID
	char *m_szICMPData;			// 用来存放整个ICMP封包
	BOOL m_bIsInitSucc;			// 初始化成功标志
private:
	static USHORT s_usPacketSeq;// 序号
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
