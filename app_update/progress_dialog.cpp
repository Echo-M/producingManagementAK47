
// progress_dialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "application.h"
#include "progress_dialog.h"
#include "../thread_inl.h"
#include "ZipArchive/ZipArchive.h"
#include <list>
#include "../graphics.h"
#include "../gui_resources.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPGRADE_DONE   WM_USER + 100

// ProgressDialog �Ի���
ProgressDialog::ProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void ProgressDialog::SetUpgradeInfo(LPCTSTR upgradeFilePath, LPCTSTR appDir, LPCTSTR version) {
  upgradeFilePath_ = upgradeFilePath;
  appDir_ = appDir;
  appVersion_ = version;
}

BEGIN_MESSAGE_MAP(ProgressDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_MESSAGE(WM_UPGRADE_DONE, &ProgressDialog::OnUpgradeDone)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// ProgressDialog ��Ϣ�������

BOOL ProgressDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

  GuiResources::GetInstance()->Init();

  loaddingBitmap_.LoadFromResource(IDB_LOADING);
  cxLoadding_ = 24;
  cyLoadding_ = 24;
  currentFrame_ = 0;
  frameCount_ = 24;

  SetTimer(1, 60, NULL);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
  upgradeThread_.Start(new MethodRunnable<ProgressDialog>(this, &ProgressDialog::UpgradeAsync));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ProgressDialog::OnPaint()
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
		BufferredPaintDC dc(this);

    CRect clientRect;
    GetClientRect(&clientRect);
    dc.FillSolidRect(clientRect, RGB(45, 108, 167));

    TCHAR szTips[] = _T("���ڸ��³���...");
    CFont *oldFont = dc.SelectObject(GuiResources::GetInstance()->GetFont(GuiResources::FONT_NORMAL));
    dc.SetTextColor(RGB(255, 255, 255));
    dc.SetBkMode(TRANSPARENT);
    CSize tipsSize = dc.GetTextExtent(szTips);
    int x = (clientRect.Width() - tipsSize.cx - cxLoadding_ - 6) / 2;
    if (loaddingBitmap_.IsValid()) {
      int bitmapX = (currentFrame_ * cxLoadding_) % loaddingBitmap_.GetWidth();
      int bitmapY = (currentFrame_ * cxLoadding_) / loaddingBitmap_.GetWidth() * cyLoadding_;
      int y = (clientRect.Height() - cyLoadding_) / 2;
      loaddingBitmap_.Draw(dc.GetSafeHdc(), bitmapX, bitmapY, cxLoadding_, cyLoadding_, x, y);
    }
    x += cxLoadding_ + 6;
    int y = (clientRect.Height() - tipsSize.cy) / 2;
    dc.TextOut(x, y, szTips);

    dc.SelectObject(oldFont);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ProgressDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD ProgressDialog::UpgradeAsync() {
  typedef std::list<CString> BackupFiles;

  CZipArchive ar;
  if (!ar.Open(upgradeFilePath_)) {
    ::PostMessage(m_hWnd, WM_UPGRADE_DONE, FALSE, 0);
    return 0;
  }

  bool copySuccess = true;
  TCHAR filePath[MAX_PATH];
  BackupFiles backupFiles;
  WORD fileCount = ar.GetCount();
  for (WORD i = 0; i < fileCount; i++) {
    CZipFileHeader *fileInfo = ar.GetFileInfo(i);
    if (fileInfo->IsDirectory()) {
      continue;
    }
    const CZipString &fileName = fileInfo->GetFileName();
    _tcscpy(filePath, appDir_);
    PathAddBackslash(filePath);
    _tcscat(filePath, fileName);
    if (PathFileExists(filePath)) {
      TCHAR backupFilePath[MAX_PATH];
      _tcscpy(backupFilePath, appDir_);
      PathAddBackslash(backupFilePath);
      _tcscat(backupFilePath, fileName);
      _tcscat(backupFilePath, _T(".bak"));
      if (PathFileExists(backupFilePath)) {
        DeleteFile(backupFilePath);
      }
      if (!MoveFile(filePath, backupFilePath)) {
        copySuccess = false;
        break;
      }
      backupFiles.push_back((LPCTSTR)fileName);
    }
    if (!ar.ExtractFile(i, appDir_)) {
      copySuccess = false;
      break;
    }
  }
  
  if (copySuccess) {
    AfxGetApp()->WriteProfileString(_T(""), _T("Version"), appVersion_);

    // remove backup files.
    for (BackupFiles::const_iterator i = backupFiles.begin(); i != backupFiles.end(); ++i) {
      TCHAR backupFilePath[MAX_PATH];
      _tcscpy(backupFilePath, appDir_);
      PathAddBackslash(backupFilePath);
      _tcscat(backupFilePath, *i);
      _tcscat(backupFilePath, _T(".bak"));

      DeleteFile(backupFilePath);
    }
  } else {
    // upgrade failed, restore backup.
    for (BackupFiles::const_iterator i = backupFiles.begin(); i != backupFiles.end(); ++i) {
      TCHAR backupFilePath[MAX_PATH];
      _tcscpy(backupFilePath, appDir_);
      PathAddBackslash(backupFilePath);
      _tcscat(backupFilePath, *i);
      _tcscat(backupFilePath, _T(".bak"));

      TCHAR filePath[MAX_PATH];
      _tcscpy(filePath, appDir_);
      PathAddBackslash(filePath);
      _tcscat(filePath, *i);

      DeleteFile(filePath);
      MoveFile(backupFilePath, filePath);
    }
  }

  ::PostMessage(m_hWnd, WM_UPGRADE_DONE, TRUE, 0);

  return 0;
}

LRESULT ProgressDialog::OnUpgradeDone(WPARAM wParam, LPARAM lParam) {
  BOOL success = (BOOL)wParam;

  TCHAR appFilePath[MAX_PATH];
  _tcscpy(appFilePath, appDir_);
  PathAddBackslash(appFilePath);
  _tcscat(appFilePath, _T("ccmdbg.exe"));
  ShellExecute(NULL, NULL, appFilePath, _T("-skipupdate"), NULL, SW_SHOW);

  EndDialog(IDOK);

  return 0;
}

void ProgressDialog::OnOK() {
  // TODO: Add your specialized code here and/or call the base class

  // CDialog::OnOK();
}


void ProgressDialog::OnCancel() {
  // TODO: Add your specialized code here and/or call the base class

  // CDialog::OnCancel();
}


void ProgressDialog::OnTimer(UINT_PTR nIDEvent) {
  currentFrame_++;
  if (currentFrame_ >= frameCount_) {
    currentFrame_ = 0;
  }
  Invalidate(FALSE);

  CDialog::OnTimer(nIDEvent);
}
