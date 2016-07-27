// RunCashDialog.cpp : implementation file
//

#include "stdafx.h"
#include "run_cash_dialog.h"
#include "run_cash_monitor.h"
#include "run_cash_monitor_inl.h"
#include "gui_resources.h"
#include "graphics.h"
#include "device_proxy.h"
#include "device_proxy_inl.h"
#include "image_view_dialog.h"
#include "waveform_view_dialog.h"
#include "select_data_grade_dialog.h"
#include "working_parameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPDATE WM_USER + 100

// RunCashDialog dialog
IMPLEMENT_DYNAMIC(RunCashDialog, CDialog)

RunCashDialog::RunCashDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RunCashDialog::IDD, pParent)
  , monitor_(NULL)
  , currentCashIndex_(-1) 
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

RunCashDialog::~RunCashDialog() 
{
}

void RunCashDialog::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_STATIC_TITLE, labelTitle_);
  DDX_Control(pDX, IDC_BUTTON_CLOSE, closeButton_);
}


BEGIN_MESSAGE_MAP(RunCashDialog, CDialog)
  ON_WM_CLOSE()
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
  ON_BN_CLICKED(IDC_BUTTON_CLOSE, &RunCashDialog::OnBnClickedButtonClose)
  ON_MESSAGE(WM_UPDATE, &RunCashDialog::OnUpdate)
  ON_LBN_SELCHANGE(IDC_CASH_LIST_VIEW, &RunCashDialog::OnLbnSelchangeListCash)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDOWN()
  ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// RunCashDialog message handlers


BOOL RunCashDialog::OnInitDialog() {
  CDialog::OnInitDialog();

  labelTitle_.SetFont(GuiResources::GetInstance()->GetFont(GuiResources::FONT_NORMAL), FALSE);
  labelTitle_.SetBkColor(RGB(54, 133, 214));
  labelTitle_.SetTextColor(RGB(255, 255, 255));

  ButtonStyle buttonStyle;
  
  // close button style
  buttonStyle.bgColor = RGB(54, 133, 214);
  buttonStyle.textColor = RGB(255, 255, 255);
  buttonStyle.bitmap.LoadFromResource(IDB_BUTTON_CLOSE);
  buttonStyle.bitmap.SetWithAlpha();
  buttonStyle.bitmap.PreMultiplyAlpha();
  closeButton_.SetStyle(BUTTON_STATE_NORMAL, buttonStyle);
  buttonStyle.bgColor = RGB(255, 64, 64);
  closeButton_.SetStyle(BUTTON_STATE_OVER, buttonStyle);
  buttonStyle.bgColor = RGB(186, 0, 0);
  closeButton_.SetStyle(BUTTON_STATE_DOWN, buttonStyle);

  CRect rectCashListView;
  GetDlgItem(IDC_PLACE_LIST_CASH)->GetWindowRect(&rectCashListView);
  ScreenToClient(&rectCashListView);
  cashListView_.Create(NULL, NULL, 
      WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS, 
      rectCashListView, this, IDC_CASH_LIST_VIEW);

  layout_.Init(m_hWnd);
  layout_.AddDlgItem(IDC_PLACE_CAPTION, AnchorLayout::TOP_LEFT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_BUTTON_CLOSE, AnchorLayout::TOP_RIGHT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_PLACE_DATA, AnchorLayout::TOP_LEFT, AnchorLayout::BOTTOM_RIGHT);
  layout_.AddAnchor(cashListView_, AnchorLayout::TOP_RIGHT, AnchorLayout::BOTTOM_RIGHT);
  layout_.AddDlgItem(IDC_PLACE_PAGE, AnchorLayout::BOTTOM_LEFT, AnchorLayout::BOTTOM_RIGHT);
  layout_.AddDlgItem(IDC_SN_IMAGE, AnchorLayout::BOTTOM_RIGHT, AnchorLayout::BOTTOM_RIGHT);

  ShowWindow(SW_MAXIMIZE);

  monitor_ = RunCashMonitor::GetInstance();
  monitor_->AddObserver(_T("RunCashDialog::OnUpdate"), m_hWnd, 0, WM_UPDATE, true);

  cashListView_.SetMonitor(monitor_);
  cashListView_.SetFocus();

  PostMessage(WM_UPDATE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void RunCashDialog::OnClose() {
  if (monitor_ != NULL) {
    monitor_->RemoveObserver(_T("RunCashDialog::Update"));
    monitor_->Release();
  }

  DeviceProxy::GetInstance()->CheckConnection();

  CDialog::OnClose();
}

void RunCashDialog::OnSize(UINT nType, int cx, int cy)
{
  CDialog::OnSize(nType, cx, cy);

  if (::IsWindow(m_hWnd)) {
    layout_.RecalcLayout();
  }
}

// �㳮�������Ĳ����ź���Ҫ����˳�������Ҫ��һ����Ӧ��ϵ lux
// ԭ˳��Ϊ���š�HD�����дš����дš��ұߴš���ߴš�IR1��IR2��IR3��IR4��IR5��IR6��EIR2��EIR3��EIR4��EIR5��EIR6�������⡢��������
// ��˳��Ϊ���š�HD����ߴš����дš����дš��ұߴš�IR6��IR5��IR4��IR3��IR2��IR1��EIR2��EIR3��EIR4��EIR5��EIR6�������⡢��������
// �����ϵ�����ֻ�д��źŷ����ı䣬IR�źŲ���
//static struct ADCChannelPos 
//{
//  int row, col;
//} g_ChannelPos[] = {
//  {0, 0},
//  {0, 1},
//  {0, 2},
//  {0, 3},
//  {0, 4},
//  {0, 5},
//  {1, 0},
//  {1, 1},
//  {1, 2},
//  {1, 3},
//  {1, 4},
//  {1, 5},
//  {2, 0},
//  {2, 1},
//  {2, 2},
//  {2, 3},
//  {2, 4},
//  {3, 0},
//  {3, 1},
//};

static struct ADCChannelPos
{
	int row, col;
} g_ChannelPos[] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 0, 4 },
	{ 0, 3 },
	{ 0, 5 },
	{ 0, 2 },
	{ 1, 5 },
	{ 1, 4 },
	{ 1, 3 },
	{ 1, 2 },
	{ 1, 1 },
	{ 1, 0 },
	{ 2, 0 },
	{ 2, 1 },
	{ 2, 2 },
	{ 2, 3 },
	{ 2, 4 },
	{ 2, 5 },
	{ 3, 0 },
	{ 3, 1 },
};

void RunCashDialog::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // ���ڻ��Ƶ��豸������

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // ʹͼ���ڹ����������о���
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // ����ͼ��
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    TRACE("RunCashDialog::OnPaint\n");
        
    BufferredPaintDC dc(this);

    CRect rectTitle;
    GetDlgItem(IDC_PLACE_CAPTION)->GetWindowRect(&rectTitle);
    ScreenToClient(&rectTitle);
    dc.FillSolidRect(rectTitle, RGB(54, 133, 214));

    CRect rectData;
    GetDlgItem(IDC_PLACE_DATA)->GetWindowRect(&rectData);
    ScreenToClient(&rectData);
    dc.FillSolidRect(rectData, RGB(221, 221, 221));    

    CRect rectSNImage;
    GetDlgItem(IDC_SN_IMAGE)->GetWindowRect(&rectSNImage);
    ScreenToClient(&rectSNImage);
    dc.FillSolidRect(rectSNImage, RGB(35, 40, 45));

    /*CRect rectPage;
    GetDlgItem(IDC_PLACE_PAGE)->GetWindowRect(&rectPage);
    ScreenToClient(&rectPage);
    dc.FillSolidRect(rectPage, RGB(255, 255, 255));*/

    // draw waveform.
    int gridWidth = rectData.Width() / 6;
    int gridHeight = rectData.Height() / 4;
    CGdiObject *oldPen = dc.SelectStockObject(WHITE_PEN);
    CGdiObject *oldBrush = dc.SelectStockObject(BLACK_BRUSH);
    dc.SetTextColor(RGB(255, 255, 255));
    dc.SetBkMode(TRANSPARENT);
    CFont *oldFont = dc.SelectObject(
        GuiResources::GetInstance()->GetFont(GuiResources::FONT_NORMAL));
    for (int i = 0; i < _countof(g_ChannelPos); i++) {
      CRect rect(0, 0, gridWidth, gridHeight);
      rect.OffsetRect(rectData.left + g_ChannelPos[i].col * gridWidth, 
          rectData.top + g_ChannelPos[i].row * gridHeight);
      dc.Rectangle(rect);
      dc.TextOut(rect.left + 2, rect.top + 2, g_ADCChannels[i].name, 
          _tcslen(g_ADCChannels[i].name));
      rect.DeflateRect(2, 2, 2, 2);
      if (monitor_ != NULL && currentCashIndex_ != -1) {
        int cashCount = monitor_->Freeze();
        if (currentCashIndex_ < cashCount) {
          Cash *cash = monitor_->GetCash(currentCashIndex_);
          int count = cash->GetADCSampleCount(g_ADCChannels[i].id);
          const short *codes = cash->GetADCSampleCodes(g_ADCChannels[i].id);
          const short *values = cash->GetADCSampleValues(g_ADCChannels[i].id);
          DrawWaveform(&dc, rect, codes, values, 
              g_ADCChannels[i].minValue, g_ADCChannels[i].maxValue, count);
        }
        monitor_->Unfreeze();
      }
    }
    // draw image.
    CRect rectImage(0, 0, 4 * gridWidth, gridHeight);
    rectImage.OffsetRect(rectData.left + 2 * gridWidth, rectData.top + 3 * gridHeight);
    dc.Rectangle(rectImage);
    rectImage.DeflateRect(6, 6, 6, 6);

    dc.SetStretchBltMode(HALFTONE);

    if (monitor_ != NULL && currentCashIndex_ != -1) {
      int cashCount = monitor_->Freeze();
      if (currentCashIndex_ < cashCount) {
        Cash *cash = monitor_->GetCash(currentCashIndex_);        
        // draw image.
        Bitmap *topImage = cash->GetTopImage();
        if (topImage != NULL && topImage->IsValid()) {
          CRect rectTopImage(rectImage);
          rectTopImage.bottom = rectImage.bottom - rectImage.Height() / 2 - 1;
          topImage->Draw(dc.GetSafeHdc(), rectTopImage);
        }

        Bitmap *bottomImage = cash->GetBottomImage();
        if (bottomImage != NULL && bottomImage->IsValid()) {
          CRect rectBottomImage(rectImage);
          rectBottomImage.top = rectImage.top + rectImage.Height() / 2 + 1;
          bottomImage->Draw(dc.GetSafeHdc(), rectBottomImage);
        }
        
        // draw sn image.
        Bitmap *snImage = cash->GetSNImage();
        if (snImage != NULL && snImage->IsValid()) {
          snImage->Draw(dc.GetSafeHdc(), rectSNImage);
        }
      }
      monitor_->Unfreeze();
    }    

    dc.SelectObject(oldPen);
    dc.SelectObject(oldBrush);
    dc.SelectObject(oldFont);
  }
}

BOOL RunCashDialog::OnEraseBkgnd(CDC* pDC) {
  return TRUE;
}

void RunCashDialog::DrawWaveform(CDC *dc, const CRect &rect, const short *codes, 
    const short *values, int minValue, int maxValue, int count) {
  if (count <= 0) {
    return;
  }

  CPen penYellow(PS_SOLID, 1, RGB(255, 255, 0));
  CPen penGreen(PS_SOLID, 1, RGB(0, 255, 0));

  ::DrawWaveform(dc, rect, codes, codes[0], codes[count - 1], count, &penYellow);
  ::DrawWaveform(dc, rect, values, minValue, maxValue, count, &penGreen);
}


void RunCashDialog::OnBnClickedButtonClose() {
  PostMessage(WM_CLOSE);
}

LRESULT RunCashDialog::OnUpdate(WPARAM, LPARAM) {
  if (GetFocus() != &cashListView_) {
    cashListView_.SetFocus();
  }
  cashListView_.Update();
  currentCashIndex_ = cashListView_.GetSelected();

  Invalidate(FALSE);  
  cashListView_.UpdateWindow();

  return 0;
}

void RunCashDialog::OnLbnSelchangeListCash() {
  currentCashIndex_ = cashListView_.GetSelected();

  Invalidate(FALSE);
}

void RunCashDialog::OnLButtonDblClk(UINT nFlags, CPoint point) {
  CRect rectData;
  GetDlgItem(IDC_PLACE_DATA)->GetWindowRect(&rectData);
  ScreenToClient(&rectData);
  if (rectData.PtInRect(point)) {
    int gridWidth = rectData.Width() / 6;
    int gridHeight = rectData.Height() / 4;
    CRect rectImage(0, 0, 4 * gridWidth, gridHeight);
    rectImage.OffsetRect(rectData.left + 2 * gridWidth, rectData.top + 3 * gridHeight);
    if (rectImage.PtInRect(point)) {
      if (monitor_ != NULL && currentCashIndex_ != -1) {
        int cashCount = monitor_->Freeze();
        if (currentCashIndex_ < cashCount) {
          ImageViewDialog dlg(monitor_, currentCashIndex_);
          dlg.DoModal();
          if (dlg.GetSelectedCashIndex() != currentCashIndex_) {
            cashListView_.SelectItem(dlg.GetSelectedCashIndex());
          }
        }
        monitor_->Unfreeze();
      }
    }

    for (int i = 0; i < _countof(g_ChannelPos); i++) {
      CRect rect(0, 0, gridWidth, gridHeight);
      rect.OffsetRect(rectData.left + g_ChannelPos[i].col * gridWidth, rectData.top + g_ChannelPos[i].row * gridHeight);
      if (rect.PtInRect(point)) {
        if (monitor_ != NULL && currentCashIndex_ != -1) {
          int cashCount = monitor_->Freeze();
          if (currentCashIndex_ < cashCount) {
            WaveformViewDialog dlg(monitor_, currentCashIndex_, i);
            dlg.DoModal();
            if (dlg.GetSelectedCashIndex() != currentCashIndex_) {
              cashListView_.SelectItem(dlg.GetSelectedCashIndex());
            }
          }
          monitor_->Unfreeze();
        }
        break;
      }
    }
  }

  CDialog::OnLButtonDblClk(nFlags, point);
}

void RunCashDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SELECT_DATA_GRADE);//����˵�
	CMenu *contextMenu = menu.GetSubMenu(0);//����Ӳ˵�ָ��
	int choice = contextMenu->TrackPopupMenu(TPM_RETURNCMD, point.x, point.y, this);

	if (choice == ID_SELECT_DATA_GRADE)
	{
		// ���Ȼ�ȡ��ǰ������
		VariableConfigBlock *config = WorkingParameters::GetInstance()->GetCurrentConfigParameters();
		int waveDataLevel = config->GetIntParameter(_T("WaveDataLevel"), 1);
		int imageDataLevel = config->GetIntParameter(_T("ImageDataLevel"), 1);
		
		// ��ʾ�Ի���
		SelectDataGradeDialog dlg;
		dlg.SetWaveDataLevel(waveDataLevel);
		dlg.SetImageDataLevel(imageDataLevel);
		if (dlg.DoModal() == IDOK)
		{
			int choiceWave = dlg.GetWaveDataGrade();
			if (choiceWave == IDC_NO_WAVE)
				waveDataLevel = 0;
			else if (choiceWave == IDC_EXIST_WAVE)
				waveDataLevel = 1;
			int choiceImage = dlg.GetImageDataGrade();
			if (choiceImage == IDC_NO_IMAGE)
				imageDataLevel = 0;
			else if (choiceImage == IDC_FULL_IMAGE)
				imageDataLevel = 1;
			else if (choiceImage == IDC_QUARTER_IMAGE)
				imageDataLevel = 2;
		}

		// ����ѡ������������
		config->SetIntParameter(_T("WaveDataLevel"), waveDataLevel);
		config->SetIntParameter(_T("ImageDataLevel"), imageDataLevel);
	}

	CDialog::OnRButtonDown(nFlags, point);
}

void RunCashDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	// TODO: Add your message handler code here and/or call default  
	//lpMMI->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	lpMMI->ptMaxSize.x = rt.Width();
	lpMMI->ptMaxSize.y = rt.Height();
	CDialog::OnGetMinMaxInfo(lpMMI);
}


