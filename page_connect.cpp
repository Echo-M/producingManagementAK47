// page_connect.cpp : implementation file
//

#include "stdafx.h"
#include "page_connect.h"
#include "gui_resources.h"
#include "device_proxy.h"
#include "thread_inl.h"
#include "working_parameters.h"
#include "working_parameters_inl.h"
#include "main_dialog.h"
#include "ping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(PageConnect, Page)

PageConnect::PageConnect(CWnd* pParent /*=NULL*/)
	: Page(PageConnect::IDD, pParent)
{

}

PageConnect::~PageConnect()
{
}

void PageConnect::DoDataExchange(CDataExchange* pDX)
{
  Page::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_STATIC_TIPS, labelTips_);
}

void PageConnect::OnEnter(int leavePageId) {
  EnableConnectionCheck(false);
  
  connectThread_.Start(new MethodRunnable<PageConnect>(this, &PageConnect::TryConnect));
}

bool PageConnect::OnLeave(int enterPageId) {
  connectThread_.Interrupt();
  connectThread_.WaitForExit(INFINITE);

  EnableConnectionCheck(true);

  return true;
}

DWORD PageConnect::TryConnect() {
  ParameterBlock *modelBlock = WorkingParameters::GetInstance()->GetCurrentModelParameters();
  ASSERT(modelBlock != NULL && !modelBlock->IsNull());
  ParameterBlock addressBlock = modelBlock->SelectBlock(_T("Config\\DeviceAddress"));
  CString deviceIP = addressBlock.GetStringParameter(_T("ip"), _T("192.168.8.131"));
  deviceIP = AfxGetApp()->GetProfileStringW(_T("Debug"), _T("Device IP"), deviceIP);
  int devicePort = addressBlock.GetIntParameter(_T("port"), 1234);
  int len = WideCharToMultiByte(CP_ACP, 0, deviceIP, -1, NULL, 0, NULL, NULL);
  char *ip = new char[len + 1];
  WideCharToMultiByte(CP_ACP, 0, deviceIP, -1, ip, len, NULL, NULL);
  static int cnt = 0;
  CPing objPing;
  PingReply reply;
  static unsigned long initTime = GetTickCount();

  while (!connectThread_.IsInterrupted()) 
  {
	// ��������
    if (DeviceProxy::GetInstance()->Connect(deviceIP, devicePort)) 
	{
		break;
    }

	//ping
	if (GetTickCount() - initTime >= 5000)
	{
		initTime = GetTickCount();
		//if (objPing.Ping((char*)deviceIP.GetBuffer(deviceIP.GetLength() + 1), &reply)) //Ping�ɹ�
		if (objPing.Ping(ip, &reply,10)) //Ping�ɹ�
		{
			TRACE("Reply from %s: bytes=%ld time=%ldms TTL=%ld\n", ip, reply.m_dwBytes, reply.m_dwRoundTripTime, reply.m_dwTTL);
		}
		else
		{
			TRACE("%d::Out of time����\n", initTime);
		}

	}

	TRACE("failed to connect����\n");
    Sleep(200);
  }
  delete ip;
  return 0;
}

BEGIN_MESSAGE_MAP(PageConnect, Page)
  ON_WM_SIZE()
END_MESSAGE_MAP()
// PageConnect message handlers


BOOL PageConnect::OnInitDialog() {
  Page::OnInitDialog();

  // TODO:  Add extra initialization here
  labelTips_.SetFont(GuiResources::GetInstance()->GetFont(GuiResources::FONT_BIG_BOLD));
  labelTips_.SetBkColor(GetBgColor());
  labelTips_.SetTextColor(RGB(68, 68, 68));
  
  layout_.Init(m_hWnd);
  layout_.AddDlgItem(IDC_STATIC_TIPS, AnchorLayout::CENTER, AnchorLayout::CENTER);
  layout_.AddDlgItem(IDC_STATIC_IMAGE, AnchorLayout::CENTER, AnchorLayout::CENTER);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void PageConnect::OnSize(UINT nType, int cx, int cy) {
  Page::OnSize(nType, cx, cy);

  if (::IsWindow(m_hWnd)) {
    layout_.RecalcLayout();
  }
}
