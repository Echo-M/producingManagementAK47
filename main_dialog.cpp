
// main_dialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "application.h"
#include "main_dialog.h"
#include "about_dialog.h"
#include "page_connect.h"
#include "page_upgrade.h"
#include "page_ir_calibration.h"
#include "page_cis_calibration.h"
#include "page_magnetic_detect.h"
#include "page_run_cash_test.h"
#include "page_finish.h"
#include "gui_resources.h"
#include "device_proxy.h"
#include "working_parameters.h"
#include "working_parameters_inl.h"
#include "run_cash_dialog.h"
#include "debug_logger.h"
#include "Computer_Information.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_DEVICE_CONNECTED     WM_USER + 100
#define WM_DEVICE_DISCONNECTED  WM_USER + 101

#define STATUS_CONNECT  1
#define STATUS_SN       2
#define STATUS_MODEL    3

#define TIMER_CHECK_CONNECTION    2
#define TIMER_DOWNLOAD_CONFIG	  3

MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MainDialog::IDD, pParent)
  , currentPage_(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

MainDialog::~MainDialog() 
{
	if (!WorkingParameters::GetInstance()->IsOffline())
	{
		KillTimer(TIMER_DOWNLOAD_CONFIG);
	}
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_STATIC_TITLE, labelTitle_);
  DDX_Control(pDX, IDC_LIST_STEP, navigationView_);
  DDX_Control(pDX, IDC_BUTTON_RUN_CASH, runCashButton_);
  DDX_Control(pDX, IDC_BUTTON_MIN, minButton_);
  DDX_Control(pDX, IDC_BUTTON_CLOSE, closeButton_);
  DDX_Control(pDX, IDC_BUTTON_CHOICE_MODEL, choiceModelButton_);
}

BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BUTTON_CLOSE, &MainDialog::OnBnClickedButtonClose)
  ON_BN_CLICKED(IDC_BUTTON_MIN, &MainDialog::OnBnClickedButtonMin)
  ON_WM_CLOSE()
  ON_BN_CLICKED(IDC_BUTTON_RUN_CASH, &MainDialog::OnBnClickedButtonRunCash)
  ON_MESSAGE(WM_DEVICE_CONNECTED, &MainDialog::OnDeviceConnected)
  ON_MESSAGE(WM_DEVICE_DISCONNECTED, &MainDialog::OnDeviceDisconnected)
  ON_LBN_SELCHANGE(IDC_LIST_STEP, &MainDialog::OnLbnSelchangeListStep)
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_BUTTON_CHOICE_MODEL, &MainDialog::OnBnClickedButtonChoiceModel)
  ON_STN_CLICKED(IDC_STATIC_TITLE, &MainDialog::OnStnClickedStaticTitle)
  ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// MainDialog ��Ϣ�������
BOOL MainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX); // ��4λ��ϵͳ�ڲ�ʹ�õ�
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);//��ô��ڲ˵��Ŀ���
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			//���һ��ˮƽ��
			pSysMenu->AppendMenu(MF_SEPARATOR);

			//IDM_ABOUTBOX��ʾҪִ�еĶ�������Ϊ�ڵ������˵���ʱ��
			//���ᷢ����Ϣ����Ϣ��nID�������ID��ͨ���ȶ�ID������Ӧ�Ķ���
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��32*32
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��16*16

  // ���ñ�������塢����ɫ��������ɫ
  labelTitle_.SetFont(GuiResources::GetInstance()->GetFont(GuiResources::FONT_NORMAL), FALSE);
  labelTitle_.SetBkColor(RGB(54, 133, 214));		// ��ɫ
  labelTitle_.SetTextColor(RGB(255, 255, 255));		// ��ɫ

  // ״̬��
  statusBar_.Create(this, IDC_PLACE_STATUS);
  statusBar_.AddPanel(STATUS_CONNECT, _T("�豸δ����"), IDB_OFF, StatusBar::PANEL_ALIGN_RIGHT);
  statusBar_.AddPanel(STATUS_SN, _T(""), 0, StatusBar::PANEL_ALIGN_RIGHT);
  statusBar_.AddPanel(STATUS_MODEL, _T(""), 0, StatusBar::PANEL_ALIGN_RIGHT);

  // ��ť��ʽ
  ButtonStyle buttonStyle;
  // run cash button style
  buttonStyle.bgColor = RGB(54, 133, 214);
  buttonStyle.textColor = RGB(255, 255, 255);
  buttonStyle.bitmap.LoadFromResource(IDB_BUTTON_RUN_CASH);
  buttonStyle.bitmap.SetWithAlpha();
  buttonStyle.bitmap.PreMultiplyAlpha();
  runCashButton_.SetStyle(BUTTON_STATE_NORMAL, buttonStyle);
  buttonStyle.bgColor = RGB(109, 173, 239);
  runCashButton_.SetStyle(BUTTON_STATE_OVER, buttonStyle);
  buttonStyle.bgColor = RGB(4, 84, 166);
  runCashButton_.SetStyle(BUTTON_STATE_DOWN, buttonStyle);

  // choice model button.
  buttonStyle.bgColor = RGB(54, 133, 214);
  buttonStyle.textColor = RGB(255, 255, 255);
  buttonStyle.bitmap.Destroy();
  choiceModelButton_.SetStyle(BUTTON_STATE_NORMAL, buttonStyle);
  buttonStyle.bgColor = RGB(109, 173, 239);
  choiceModelButton_.SetStyle(BUTTON_STATE_OVER, buttonStyle);
  buttonStyle.bgColor = RGB(4, 84, 166);
  choiceModelButton_.SetStyle(BUTTON_STATE_DOWN, buttonStyle);

  ParameterBlock *currentModel = WorkingParameters::GetInstance()->GetCurrentModelParameters();
  ASSERT(currentModel != NULL);
  choiceModelButton_.SetWindowText(currentModel->GetStringParameter(_T("name"), NULL));

  // minimize button style
  buttonStyle.bgColor = RGB(54, 133, 214);
  buttonStyle.textColor = RGB(255, 255, 255);
  buttonStyle.bitmap.LoadFromResource(IDB_BUTTON_MINIMIZE);
  buttonStyle.bitmap.SetWithAlpha();
  buttonStyle.bitmap.PreMultiplyAlpha();
  minButton_.SetStyle(BUTTON_STATE_NORMAL, buttonStyle);
  buttonStyle.bgColor = RGB(109, 173, 239);
  minButton_.SetStyle(BUTTON_STATE_OVER, buttonStyle);
  buttonStyle.bgColor = RGB(4, 84, 166);
  minButton_.SetStyle(BUTTON_STATE_DOWN, buttonStyle);

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

  InitPages();
  InitNavigationView();

  layout_.Init(m_hWnd);
  layout_.AddDlgItem(IDC_PLACE_CAPTION, AnchorLayout::TOP_LEFT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_BUTTON_CHOICE_MODEL, AnchorLayout::TOP_RIGHT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_BUTTON_RUN_CASH, AnchorLayout::TOP_RIGHT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_BUTTON_MIN, AnchorLayout::TOP_RIGHT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_BUTTON_CLOSE, AnchorLayout::TOP_RIGHT, AnchorLayout::TOP_RIGHT);
  layout_.AddDlgItem(IDC_LIST_STEP, AnchorLayout::TOP_LEFT, AnchorLayout::BOTTOM_LEFT);
  layout_.AddDlgItem(IDC_PLACE_PAGE, AnchorLayout::TOP_LEFT, AnchorLayout::BOTTOM_RIGHT);
  layout_.AddAnchor(statusBar_.m_hWnd, AnchorLayout::BOTTOM_LEFT, AnchorLayout::BOTTOM_RIGHT);

	ShowWindow(SW_MAXIMIZE);

  DeviceProxy::GetInstance()->AddObserver(_T("MainDialog::OnDeviceConnected"), m_hWnd, DeviceProxy::SUBJECT_CONNECTED, WM_DEVICE_CONNECTED);
  DeviceProxy::GetInstance()->AddObserver(_T("MainDialog::OnDeviceDisconnected"), m_hWnd, DeviceProxy::SUBJECT_DISCONNECTED, WM_DEVICE_DISCONNECTED);

  //ShowPage(IDD_PAGE_CONNECT);
  ShowPage(IDD_PAGE_FINISH);

  // ��������ߵ�¼����ôÿ30��������һ�οɱ������ļ�
  if (!WorkingParameters::GetInstance()->IsOffline())
  {
	  SetTimer(TIMER_DOWNLOAD_CONFIG, 1800000, NULL);
  }

  // ���ݵ�ǰ��Ψһʶ���롢ģʽ��Ȩ�޸��ı���
  int permission = WorkingParameters::GetInstance()->GetCurrentPermission();
  TCHAR terminalID[200] = { 0 };
  GetTerminalID(terminalID);
  VariableConfigBlock *pConfig = WorkingParameters::GetInstance()->GetCurrentConfigParameters();
  CString mode = pConfig->GetCurrentMode();
  CRect rc;
  GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(&rc);
  GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(CRect(rc.left, rc.top, rc.left + 1200, rc.top + 30), SWP_NOZORDER | SWP_NOMOVE);
  CString title;
  if (permission == 2)
  {
	  if (WorkingParameters::GetInstance()->IsOffline())
	  {
		  title.Format(_T("׿Ŀ�㳮����������ϵͳ �豸ʶ���룺%s ��ǰ״̬�����ߵ�¼ ��ǰģʽ��%s ��ǰȨ�ޣ�����Ա"), terminalID, mode);
	  }
	  else
	  {
		  title.Format(_T("׿Ŀ�㳮����������ϵͳ �豸ʶ���룺%s ��ǰ״̬�����ߵ�¼ ��ǰģʽ��%s ��ǰȨ�ޣ�����Ա"), terminalID, mode);
	  }
  }
  else
  {
	  if (WorkingParameters::GetInstance()->IsOffline())
	  {
		  title.Format(_T("׿Ŀ�㳮����������ϵͳ �豸ʶ���룺%s ��ǰ״̬�����ߵ�¼ ��ǰģʽ��%s ��ǰȨ�ޣ����߹���"), terminalID, mode);
	  }
	  else
	  {
		  title.Format(_T("׿Ŀ�㳮����������ϵͳ �豸ʶ���룺%s ��ǰ״̬�����ߵ�¼ ��ǰģʽ��%s ��ǰȨ�ޣ����߹���"), terminalID, mode);
	  }
  }
  GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(title);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void MainDialog::InitNavigationView() {
  navigationView_.AddItem(STEP_UPGRADE, IDS_UPGRADE, false);
  navigationView_.AddItem(STEP_IR_CALIBRATION, IDS_IR_CALIBRATION, false);
  navigationView_.AddItem(STEP_MH_DETECT, IDS_MH_DETECT, false);
  navigationView_.AddItem(STEP_CIS_CALIBRATION, IDS_CIS_CALIBRATION, false);
  //navigationView_.AddItem(STEP_STUDY, IDS_STUDY, false);
  navigationView_.AddItem(STEP_RUN_CASH_TEST, IDS_RUN_CASH, false);
  navigationView_.AddItem(STEP_FINISH, IDS_FINISH, false);
}

void MainDialog::InitPages() {
  AddPage(IDD_PAGE_CONNECT, new PageConnect(this));
  AddPage(IDD_PAGE_UPGRADE, new PageUpgrade(this));
  AddPage(IDD_PAGE_IR_CALIBRATION, new PageIRCalibration(this));
  AddPage(IDD_PAGE_CIS_CALIBRATION, new PageCISCalibration(this));
  AddPage(IDD_PAGE_MAGNETIC_DETECT, new PageMagneticDetect(this));
  AddPage(IDD_PAGE_RUN_CASH_TEST, new PageRunCashTest(this));
  AddPage(IDD_PAGE_FINISH, new PageFinish(this));
}

void MainDialog::AddPage(UINT id, Page *page) {
  PageItem item = { id, page };
  pages_.push_back(item);
}

int MainDialog::GetCurrentPageId() const {
  return currentPage_ == -1 ? 0 : pages_[currentPage_].id;
}

bool MainDialog::ShowPage(UINT pageId) 
{
	//ͨ��pageId��õ�ǰ�ǵڼ����
  int pageIndex = -1;
  for (int i = 0; i < (int)pages_.size(); i++) 
  {
    if (pages_[i].id == pageId) 
	{
      pageIndex = i;
      break;
    }
  }

  if (currentPage_ != pageIndex) 
  {
    int leavePageId = 0;
    if (currentPage_ != -1) 
	{
      if (!pages_[currentPage_].page->OnLeave(pageId)) {
        return false;
      }      
      pages_[currentPage_].page->ShowWindow(SW_HIDE);
      leavePageId = pages_[currentPage_].id;
    }
    if (pageIndex != -1) 
	{
      if (!::IsWindow(pages_[pageIndex].page->m_hWnd)) {
        pages_[pageIndex].page->Create(pageId, this);
      }      
      CRect rectPlace;
      GetDlgItem(IDC_PLACE_PAGE)->GetWindowRect(&rectPlace);
      ScreenToClient(&rectPlace);
      pages_[pageIndex].page->SetWindowPos(NULL,
          rectPlace.left, rectPlace.top,
          rectPlace.Width(), rectPlace.Height(),
          SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);
      pages_[pageIndex].page->OnEnter(leavePageId);
    }
    currentPage_ = pageIndex;
  }
  return true;
}

void MainDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void MainDialog::OnPaint()
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
    CPaintDC dc(this);

    CRect rectTitle;
    GetDlgItem(IDC_PLACE_CAPTION)->GetWindowRect(&rectTitle);
    ScreenToClient(&rectTitle);
    dc.FillSolidRect(rectTitle, RGB(54, 133, 214));
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR MainDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void MainDialog::OnSize(UINT nType, int cx, int cy)
{
  CDialog::OnSize(nType, cx, cy);

  if (::IsWindow(m_hWnd)) {
    layout_.RecalcLayout();
  }

  if (currentPage_ != -1 && ::IsWindow(pages_[currentPage_].page->m_hWnd)) {
    CRect rectPlace;
    GetDlgItem(IDC_PLACE_PAGE)->GetWindowRect(&rectPlace);
    ScreenToClient(&rectPlace);
    pages_[currentPage_].page->SetWindowPos(NULL,
      rectPlace.left, rectPlace.top,
      rectPlace.Width(), rectPlace.Height(),
      SWP_NOACTIVATE | SWP_NOZORDER);
  }
}


BOOL MainDialog::OnEraseBkgnd(CDC* pDC)
{
  return TRUE;
}


void MainDialog::OnDestroy()
{
  CDialog::OnDestroy();

  for (Pages::iterator i = pages_.begin(); i != pages_.end(); ++i) {
    if ((*i).page->m_hWnd != NULL) {
      (*i).page->DestroyWindow();
    }
    delete (*i).page;
  }
  pages_.clear();
}

void MainDialog::OnBnClickedButtonClose() {
  PostMessage(WM_CLOSE);
}

void MainDialog::OnBnClickedButtonMin() {
  ShowWindow(SW_MINIMIZE);
}

void MainDialog::OnClose() {
  if (currentPage_ != -1) {
    if (!pages_[currentPage_].page->OnLeave(0)) {
      return;
    }
  }

  CDialog::OnClose();
}

LRESULT MainDialog::OnDeviceConnected(WPARAM, LPARAM) {
  int nextStep = STEP_UPGRADE;
  
  DeviceInfo info;
  ZeroMemory(&info, sizeof(info));
  VERIFY(DeviceProxy::GetInstance()->GetDeviceInfo(&info));
  ParameterBlock *modelBlock = WorkingParameters::GetInstance()->GetCurrentModelParameters();
  ASSERT(modelBlock != NULL && !modelBlock->IsNull());
  ParameterBlock firmwareBlock = modelBlock->SelectBlock(_T("Firmware"));
  CString firmwareVersion = firmwareBlock.GetStringParameter(_T("version"), NULL);
  if (firmwareVersion.CompareNoCase(CA2T(info.firmwareVersion)) == 0) {
    if (info.debugState[0] & DEBUG_RUN_CASH_TEST_DONE) {
      nextStep = STEP_FINISH;
    }/* else if (info.debugState[0] & DEBUG_TAPE_STUDY_DONE) {
      nextStep = STEP_RUN_CASH_TEST;
    }*/ else if (info.debugState[0] & DEBUG_CIS_CALIBRATION_DONE) {
      nextStep = STEP_RUN_CASH_TEST;
    } else if (info.debugState[0] & DEBUG_MASTER_SIGNAL_DETECT_DONE) {
      nextStep = STEP_CIS_CALIBRATION;
    } else if (info.debugState[0] & DEBUG_IR_CALIBRATION_DONE) {
      nextStep = STEP_MH_DETECT;
    } else if (info.debugState[0] & DEBUG_UPGRADE_DONE) {
      nextStep = STEP_IR_CALIBRATION;
    }
  }

  GotoStep(nextStep);
  
  statusBar_.SetPanel(STATUS_CONNECT, _T("�豸������"), IDB_ON);
  CString model;
  model.Format(_T("�ͺ�:%s"), CA2T(info.model));
  statusBar_.SetPanel(STATUS_MODEL, model, 0);
  CString sn;
  sn.Format(_T("���к�:%s"), CA2T(info.sn));
  statusBar_.SetPanel(STATUS_SN, sn, 0);
  
  if (info.sn[0] == '\0') {
    if (inputSNDialog_.DoModal() == IDOK) {
      DeviceProxy::GetInstance()->GetDeviceInfo(&info);
      sn.Format(_T("���к�:%s"), CA2T(info.sn));
      statusBar_.SetPanel(STATUS_SN, sn, 0);
    }
  }

  DebugLogger::GetInstance()->StartDebug();

  return 0;
}

void MainDialog::OnBnClickedButtonRunCash() {
  RunCashDialog dlg;
  dlg.DoModal();
}

LRESULT MainDialog::OnDeviceDisconnected(WPARAM, LPARAM) {
  ShowPage(IDD_PAGE_CONNECT);

  statusBar_.SetPanel(STATUS_CONNECT, _T("�豸δ����"), IDB_OFF);
  statusBar_.SetPanel(STATUS_MODEL, _T(""), 0);
  statusBar_.SetPanel(STATUS_SN, _T(""), 0);

  navigationView_.DisableAllItems();

  return 0;
}

void MainDialog::OnLbnSelchangeListStep() {
  int selected = navigationView_.GetCurSel();
  
  GotoStep(selected);
}

bool MainDialog::GotoStep(int step) {
	int selectedPageId = GetCurrentPageId();
	switch (step) {
	case STEP_UPGRADE:
		selectedPageId = IDD_PAGE_UPGRADE;
		break;
	case STEP_IR_CALIBRATION:
		selectedPageId = IDD_PAGE_IR_CALIBRATION;
		break;
	case STEP_MH_DETECT:
		selectedPageId = IDD_PAGE_MAGNETIC_DETECT;
		break;
	case STEP_CIS_CALIBRATION:
		selectedPageId = IDD_PAGE_CIS_CALIBRATION;
		break;
	case STEP_RUN_CASH_TEST:
		selectedPageId = IDD_PAGE_RUN_CASH_TEST;
		break;
	case STEP_FINISH:
		selectedPageId = IDD_PAGE_FINISH;
		break;
	}

	if (ShowPage(selectedPageId)) {
		navigationView_.DisableAllItems();

		//bool expertMode = (AfxGetApp()->GetProfileInt(_T(""), _T("Expert Mode"), 1) != 0);
		// ���ڸ�Ϊ�������ļ���ȷ�������Ƿ����ת lux
		VariableConfigBlock *config = WorkingParameters::GetInstance()->GetCurrentConfigParameters();
		int expertMode = config->GetIntParameter(_T("SkipStep"), 0);
		// ���ǹ���Ȩ���²���������ת
		if (WorkingParameters::GetInstance()->GetCurrentPermission() == 3)
		{
			expertMode = 0;
		}
		if (expertMode != 0) {
			for (int i = 0; i < navigationView_.GetCount(); i++) {
				navigationView_.EnableItem(i, TRUE);
			}
		}
		else {
			for (int i = 0; i <= step; i++) {
				navigationView_.EnableItem(i, TRUE);
			}
		}
		navigationView_.SetCurSel(step);

		return true;
	}

	return false;
}

void MainDialog::EnableConnectionCheck(bool enabled) {
  if (enabled) {
    SetTimer(TIMER_CHECK_CONNECTION, 500, NULL);
  } else {
    KillTimer(TIMER_CHECK_CONNECTION);
  }
}

void MainDialog::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent)
	{
	case TIMER_CHECK_CONNECTION:
		DeviceProxy::GetInstance()->CheckConnection();
		break;
	case TIMER_DOWNLOAD_CONFIG:
		// �ӷ��������ؿɱ������ļ��������Ƿ�ɹ� lux
		WorkingParameters::GetInstance()->LoadConfigFromServer();
		break;
	default:
		break;
	}

  CDialog::OnTimer(nIDEvent);
}

void MainDialog::OnBnClickedButtonChoiceModel() {
  CMenu menu;
  menu.CreatePopupMenu();
  ParameterBlock device = WorkingParameters::GetInstance()->SelectBlock(_T("Device"));
  int currentModelId = WorkingParameters::GetInstance()->GetCurrentModelId();
  ParameterBlock model = device.GetFirstChildBlock();
  while (!model.IsNull()) {
    if (model.GetName() == _T("Model")) {
      CString name = model.GetStringParameter(_T("name"), NULL);
      int id = model.GetIntParameter(_T("id"), -1);
      int flags = MF_STRING;
      if (id == currentModelId) {
        flags |= MF_CHECKED;
      }
      menu.AppendMenu(flags, id, name);
    }
    model = model.GetNextBlock();
  }
  CRect wndRect;
  choiceModelButton_.GetWindowRect(&wndRect);
  int id = menu.TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, 
      wndRect.left, wndRect.bottom, this);
  if (id != 0) {
    WorkingParameters::GetInstance()->SetCurrentModel(id);
    ParameterBlock *currentModel = WorkingParameters::GetInstance()->GetCurrentModelParameters();
    ASSERT(currentModel != NULL);
    choiceModelButton_.SetWindowText(currentModel->GetStringParameter(_T("name"), NULL));
  }
}


void MainDialog::OnStnClickedStaticTitle()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void MainDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	// TODO: Add your message handler code here and/or call default  
	//lpMMI->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	lpMMI->ptMaxSize.x = rt.Width();
	lpMMI->ptMaxSize.y = rt.Height();
	CDialog::OnGetMinMaxInfo(lpMMI);
}
