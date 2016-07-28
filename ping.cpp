
#include "stdafx.h"
#include "ping.h"

USHORT CPing::s_usPacketSeq = 0;

CPing::CPing() :
m_szICMPData(NULL),
m_bIsInitSucc(FALSE)
{
	WSADATA WSAData;
	::WSAStartup(MAKEWORD(1, 1), &WSAData);// 加载Winsock库

	m_event = WSACreateEvent();
	m_usCurrentProcID = (USHORT)GetCurrentProcessId();// 获取当前进程ID

	if ((m_sockRaw = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) != SOCKET_ERROR) // 若套接字创建成功
	{
		WSAEventSelect(m_sockRaw, m_event, FD_READ);
		m_bIsInitSucc = TRUE;

		m_szICMPData = new char[DEF_PACKET_SIZE + sizeof(ICMPHeader)];

		if (m_szICMPData == NULL)
		{
			m_bIsInitSucc = FALSE;
		}
	}
}

CPing::~CPing()
{
	WSACleanup();

	if (NULL != m_szICMPData)
	{
		free(m_szICMPData);
		m_szICMPData = NULL;
	}
}

BOOL CPing::Ping(DWORD dwDestIP, PingReply *pPingReply, DWORD dwTimeout)
{
	return PingCore(dwDestIP, pPingReply, dwTimeout);
}

BOOL CPing::Ping(char *szDestIP, PingReply *pPingReply, DWORD dwTimeout)
{
	if (NULL != szDestIP)
	{
		return PingCore(inet_addr(szDestIP), pPingReply, dwTimeout);
	}
	return FALSE;
}

BOOL CPing::PingCore(DWORD dwDestIP, PingReply *pPingReply, DWORD dwTimeout)
{
	//判断初始化是否成功
	if (!m_bIsInitSucc)
	{
		return FALSE;
	}

	//配置SOCKET
	sockaddr_in sockaddrDest;
	sockaddrDest.sin_family = AF_INET;
	sockaddrDest.sin_port = htons(0);
	sockaddrDest.sin_addr.s_addr = dwDestIP;
	int nSockaddrDestSize = sizeof(sockaddrDest);
	unsigned long ulSendTimestamp = GetTickCountCalibrate();

	//构建ICMP包
	memset(m_szICMPData, 0, DEF_PACKET_SIZE + sizeof(ICMPHeader)); // 数据部分随意
	// 先填充报头
	ICMPHeader *pICMPHeader = (ICMPHeader*)m_szICMPData;
	pICMPHeader->m_byType = ECHO_REQUEST; // 请求回显
	pICMPHeader->m_byCode = 0; // 请求回显
	pICMPHeader->m_usID = m_usCurrentProcID; // 当前进程ID
	pICMPHeader->m_usSeq = ++s_usPacketSeq; // 序号
	pICMPHeader->m_ulTimeStamp = GetTickCountCalibrate(); // 时间戳
	pICMPHeader->m_usChecksum = CalCheckSum((USHORT*)m_szICMPData, DEF_PACKET_SIZE + sizeof(ICMPHeader)); //校验和

	//发送ICMP报文
	if (sendto(m_sockRaw, m_szICMPData, DEF_PACKET_SIZE + sizeof(ICMPHeader), 0, (sockaddr*)&sockaddrDest, nSockaddrDestSize) == SOCKET_ERROR)
	{
		return FALSE;
	}

	//判断是否需要接收相应报文
	if (pPingReply == NULL) // 若不需要接收回显应答，则可到此为止
	{
		return TRUE;
	}

	char recvbuf[256] = { "\0" };
	while (TRUE)
	{
		//接收响应报文
		if (WSAWaitForMultipleEvents(1, &m_event, FALSE, 100, FALSE) != WSA_WAIT_TIMEOUT)
		{
			WSANETWORKEVENTS netEvent;
			WSAEnumNetworkEvents(m_sockRaw, m_event, &netEvent);

			if (netEvent.lNetworkEvents & FD_READ)
			{
				ULONG nRecvTimestamp = GetTickCountCalibrate();
				int nPacketSize = recvfrom(m_sockRaw, recvbuf, 256, 0, (sockaddr*)&sockaddrDest, &nSockaddrDestSize);
				if (nPacketSize != SOCKET_ERROR)
				{
					IPHeader *pIPHeader = (IPHeader*)recvbuf;
					USHORT usIPHeaderLen = (USHORT)((pIPHeader->m_byVerHLen & 0x0f) * 4);
					ICMPHeader *pICMPHeader = (ICMPHeader*)(recvbuf + usIPHeaderLen);

					if (pICMPHeader->m_usID == m_usCurrentProcID //是当前进程发出的报文
						&& pICMPHeader->m_byType == ECHO_REPLY //是ICMP响应报文
						&& pICMPHeader->m_usSeq == s_usPacketSeq //是本次请求报文的响应报文
						)
					{
						pPingReply->m_usSeq = s_usPacketSeq;
						pPingReply->m_dwRoundTripTime = nRecvTimestamp - pICMPHeader->m_ulTimeStamp;
						pPingReply->m_dwBytes = nPacketSize - usIPHeaderLen - sizeof(ICMPHeader);
						pPingReply->m_dwTTL = pIPHeader->m_byTTL;
						return TRUE;
					}
				}
			}
		}
		//超时
		if (GetTickCountCalibrate() - ulSendTimestamp >= dwTimeout)
		{
			return FALSE;
		}
	}
}

USHORT CPing::CalCheckSum(USHORT *pBuffer, int nSize)
{
	unsigned long ulCheckSum = 0;
	while (nSize > 1)
	{
		ulCheckSum += *pBuffer++;
		nSize -= sizeof(USHORT);
	}
	if (nSize)
	{
		ulCheckSum += *(UCHAR*)pBuffer;
	}

	ulCheckSum = (ulCheckSum >> 16) + (ulCheckSum & 0xffff);
	ulCheckSum += (ulCheckSum >> 16);

	return (USHORT)(~ulCheckSum);
}

ULONG CPing::GetTickCountCalibrate()
{
	static ULONG s_ulFirstCallTick = 0;
	static LONGLONG s_ullFirstCallTickMS = 0;

	SYSTEMTIME systemtime;
	FILETIME filetime;
	GetLocalTime(&systemtime);
	SystemTimeToFileTime(&systemtime, &filetime);
	LARGE_INTEGER liCurrentTime;
	liCurrentTime.HighPart = filetime.dwHighDateTime;
	liCurrentTime.LowPart = filetime.dwLowDateTime;
	LONGLONG llCurrentTimeMS = liCurrentTime.QuadPart / 10000;

	if (s_ulFirstCallTick == 0)
	{
		s_ulFirstCallTick = GetTickCount();
	}
	if (s_ullFirstCallTickMS == 0)
	{
		s_ullFirstCallTickMS = llCurrentTimeMS;
	}

	return s_ulFirstCallTick + (ULONG)(llCurrentTimeMS - s_ullFirstCallTickMS);
}