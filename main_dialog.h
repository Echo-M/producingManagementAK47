
// main_dialog.h : 头文件
//

#pragma once

#include <vector>
#include "anchor_layout.h"
#include "label.h"
#include "navigation_view.h"
#include "status_bar.h"
#include "button.h"
#include "resource.h"
#include "input_sn_dialog.h"

// MainDialog 对话框
class Page;

enum DebugStep {
  STEP_UPGRADE = 0, // 升级
  STEP_IR_CALIBRATION, // 红外校准
  STEP_MH_DETECT, // 大磁检测
  STEP_CIS_CALIBRATION, // CIS校准
  //STEP_STUDY = 4,
  STEP_RUN_CASH_TEST, // 走钞测试
  STEP_FINISH // 完成
};

class MainDialog : public CDialog
{
// 构造
public:
	MainDialog(CWnd* pParent = NULL);	// 标准构造函数
  virtual ~MainDialog();

// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG };

  bool ShowPage(UINT pageId);  
  bool GotoStep(int step);
  void EnableConnectionCheck(bool enabled);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
  struct PageItem {
    UINT  id;
    Page  *page;
  };
  typedef std::vector<PageItem> Pages;

  HICON           m_hIcon;
  AnchorLayout    layout_;
  Label           labelTitle_;
  NavigationView  navigationView_;
  Pages           pages_;
  int             currentPage_;
  StatusBar       statusBar_;
  Button          closeButton_;
  Button          minButton_;
  Button          runCashButton_;
  Button          choiceModelButton_;
  InputSNDialog   inputSNDialog_;

  void InitNavigationView();
  void InitPages();
  void AddPage(UINT id, Page *page);
  int GetCurrentPageId() const;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

  DECLARE_MESSAGE_MAP()

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnDestroy();
  afx_msg void OnBnClickedButtonClose();
  afx_msg void OnBnClickedButtonMin();
  afx_msg void OnClose();
  afx_msg LRESULT OnDeviceConnected(WPARAM, LPARAM);
  afx_msg LRESULT OnDeviceDisconnected(WPARAM, LPARAM);
  afx_msg void OnBnClickedButtonRunCash();
  afx_msg void OnLbnSelchangeListStep();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
public:
  afx_msg void OnBnClickedButtonChoiceModel();
  afx_msg void OnStnClickedStaticTitle();
};
