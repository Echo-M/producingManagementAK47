#pragma once

#include "resource.h"
#include "button.h"
#include "thread.h"
#include "label.h"
#include "bitmap.h"
#include "afxwin.h"

// LoginDialog

class LoginDialog : public CDialog {
	DECLARE_DYNAMIC(LoginDialog)

public:
	LoginDialog();
	virtual ~LoginDialog();

	enum {IDD=IDD_LOGIN};

  virtual BOOL OnInitDialog();

protected:
  Button    loginButton_;
  Button    closeButton_;
  Label     userNameLabel_;
  Label     passwordLabel_;
  CString   userName_;
  CString   password_;
  Bitmap    header_;
  Button	passwordCheck_;

  DWORD Login();

  virtual void DoDataExchange(CDataExchange* pDX);
  virtual void OnOK();
  virtual void OnCancel();

  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  
	DECLARE_MESSAGE_MAP()
};


