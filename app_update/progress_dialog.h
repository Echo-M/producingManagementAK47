
// progress_dialog.h : ͷ�ļ�
//

#pragma once

#include "../thread.h"
#include "../bitmap.h"

// ProgressDialog �Ի���
class ProgressDialog : public CDialog
{
// ����
public:
	ProgressDialog(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_APP_UPDATE_DIALOG };

  void SetUpgradeInfo(LPCTSTR upgradeFilePath, LPCTSTR appDir, LPCTSTR version);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

  Bitmap    loaddingBitmap_;
  int       cxLoadding_, cyLoadding_;
  int       currentFrame_;
  int       frameCount_;
  Thread    upgradeThread_;

  DWORD UpgradeAsync();

// ʵ��
protected:
	HICON m_hIcon;
  CString upgradeFilePath_;
  CString appDir_;
  CString appVersion_;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg LRESULT OnUpgradeDone(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
  virtual void OnOK();
  virtual void OnCancel();
public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
};
