#pragma once

#include "page.h"
#include "resource.h"
#include "bitmap.h"
#include "afxwin.h"
#include "anchor_layout.h"

// PageFinish dialog
class PageFinish : public Page
{
	DECLARE_DYNAMIC(PageFinish)

public:
	PageFinish(CWnd* pParent = NULL);   // standard constructor
	virtual ~PageFinish();

// Dialog Data
	enum { IDD = IDD_PAGE_FINISH };

protected:
	Bitmap    icon_;
	AnchorLayout layout_;

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void DrawClient(CDC &dc, const CRect &rect);
  afx_msg void OnBnClickedButtonOk();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

public:
  virtual BOOL OnInitDialog();
  afx_msg void OnSize(UINT nType, int cx, int cy);
};
