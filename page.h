#pragma once

enum 
{
  HEADER_STYLE_INFO,	//信息
  HEADER_STYLE_ERROR,	//错误
  HEADER_STYLE_OK,		//确定
};

struct HeaderButton 
{
  int id;
  int textResId;
};

class Button;

class Page : public CDialog 
{
	DECLARE_DYNAMIC(Page)

public:
	// standard constructor 根据模板序号来创建页面
	Page(UINT templateId, CWnd* pParent = NULL);
	virtual ~Page();

	virtual void OnEnter(int leavePageId);
	virtual bool OnLeave(int enterPageId);

	COLORREF GetBgColor() { return RGB(255, 255, 255); }

	void SetHeaderText(int textResId);
	void SetHeaderSubText(int textResId);
	void SetHeaderButtons(HeaderButton *buttons, int count);
	void SetHeaderInfo(int style, int textResId, int subTextResId);
	void SetHeaderInfo(int style, LPCTSTR text, LPCTSTR subText);
	void SetHeaderButtonText(int id, int textResId);
	void ShowHeaderButton(int id, bool visibility);
	void ShowHeader(bool visibility);

	bool ShowPage(int pageId);
	bool GotoStep(int step);
	void EnableConnectionCheck(bool enabled);

protected:
	struct Header // 页眉
	{
		int           icon;			// 图标
		CString       text;			// 文本
		CString       subText;		// 下一步的文本
		COLORREF      bgColor;		// 背景色
		COLORREF      textColor;	// 字体颜色
		Button        *buttons[3];	// 按钮（3个）
		bool          visible;		// 可见
		int           height;		// 高度
	};
	enum 
	{
		ICON_INFO,
		ICON_OK,
		ICON_ERROR,
	};

	Header              header_;
	static CImageList   imageList_;

	void DrawHeader(CDC &dc);
	virtual void DrawClient(CDC &dc, const CRect &rect);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void LayoutHeaderButtons();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
