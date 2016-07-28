// page_finish.cpp : implementation file
//

#include "stdafx.h"
#include "page_finish.h"
#include "gui_resources.h"
#include "device_proxy.h"
#include "device_proxy_inl.h"
#include "gui_resources.h"
#include "debug_logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// PageFinish dialog

IMPLEMENT_DYNAMIC(PageFinish, Page)

PageFinish::PageFinish(CWnd* pParent /*=NULL*/)
	: Page(PageFinish::IDD, pParent) {
}

PageFinish::~PageFinish() {
}

void PageFinish::DoDataExchange(CDataExchange* pDX) {
	Page::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PageFinish, Page)
  ON_WM_SIZE()
  ON_BN_CLICKED(IDC_BUTTON_OK, &PageFinish::OnBnClickedButtonOk)
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// PageFinish message handlers
BOOL PageFinish::OnInitDialog() {
  Page::OnInitDialog();

  // TODO:  Add extra initialization here
  icon_.LoadFromResource(IDB_DEBUG_FINISH);

  SetHeaderInfo(HEADER_STYLE_INFO, IDS_PROMPT_DEBUG_FINISH, 0);
  HeaderButton buttons = { IDC_BUTTON_OK, IDS_BUTTON_OK };
  SetHeaderButtons(&buttons, 1);
  ShowHeader(true);

  GetDlgItem(IDC_EDIT_AGING_TIME)->SetWindowText(_T("1"));
  GetDlgItem(IDC_EDIT_AGING_TIME)->SetFocus();

  layout_.Init(m_hWnd);
  layout_.AddDlgItem(IDC_STATIC_AGING_TIME, AnchorLayout::TOP_CENTER, AnchorLayout::TOP_CENTER);
  layout_.AddDlgItem(IDC_EDIT_AGING_TIME, AnchorLayout::TOP_CENTER, AnchorLayout::TOP_CENTER);
  layout_.AddDlgItem(IDC_STATIC_AGING_TIME_UNIT, AnchorLayout::TOP_CENTER, AnchorLayout::TOP_CENTER);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void PageFinish::OnSize(UINT nType, int cx, int cy) {
  Page::OnSize(nType, cx, cy);

  // TODO: Add your message handler code here
  if (::IsWindow(m_hWnd)) {
	  layout_.RecalcLayout();
  }
}

void PageFinish::OnBnClickedButtonOk() 
{
	CString timeStr;
	GetDlgItem(IDC_EDIT_AGING_TIME)->GetWindowText(timeStr);

	int time;
	swscanf(timeStr, _T("%d"), &time);

	DeviceProxy::GetInstance()->SetAgingTime(&time);

    DebugLogger::GetInstance()->EndDebug();

    DeviceProxy::GetInstance()->DisableDebug();
}

void PageFinish::DrawClient(CDC &dc, const CRect &rect) {
  CFont *oldFont = dc.SelectObject(GuiResources::GetInstance()->GetFont(GuiResources::FONT_BIG_BOLD));
  CString text;
  text.LoadString(IDS_PROMPT_DEBUG_FINISH);
  CSize textSize = dc.GetTextExtent(text);
  int contentWidth = 48 + icon_.GetWidth() + 16 + textSize.cx + 48;
  int contentHeight = 24 + icon_.GetHeight() + 24;
  int x = rect.left + (rect.Width() - contentWidth) / 2;
  int y = rect.top + (rect.Height() - contentHeight) / 2;
  CRect contentRect(x, y, x + contentWidth, y + contentHeight);
  dc.FillSolidRect(contentRect, RGB(244,244,244));
  dc.Draw3dRect(contentRect, RGB(227,227,227), RGB(227,227,227));
  icon_.Draw(dc.GetSafeHdc(), x + 48, y + 24);
  dc.SetTextColor(RGB(0, 0, 0));
  dc.SetBkMode(TRANSPARENT);
  dc.TextOut(x + 48 + icon_.GetWidth() + 16, rect.top + (rect.Height() - textSize.cy) / 2, text);
  dc.SelectObject(oldFont);
}

HBRUSH PageFinish::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	COLORREF backColor = RGB(255, 255, 255); // ��ɫ
	pDC->SetBkMode(TRANSPARENT); //���ÿؼ�����͸��

	// �ж����ǲ�����Ҫ�ĵĿؼ�ID 
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_AGING_TIME || pWnd->GetDlgCtrlID() == IDC_STATIC_AGING_TIME_UNIT)
	{
		pDC->SetBkColor(backColor);
	}

	return CreateSolidBrush(backColor); //��������ˢ��
}