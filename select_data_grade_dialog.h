#pragma once
#include "resource.h"

// select_data_grade_dialog 对话框
//; “WaveDataLevel”即设置点钞机上传主控数据的级别。
//; 0 = 不上传，1 = 上传
//; “ImageDataLevel”即设置点钞机上传图像数据的级别。
//; 0 = 不上传，1 = 上传全图，2 = 上传1 / 4图

class SelectDataGradeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SelectDataGradeDialog)

public:
	SelectDataGradeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SelectDataGradeDialog();

	//获取radio按钮
	int GetWaveDataGrade(){ return m_choiceWave; }
	void SetWaveDataLevel(int level)
	{
		if (level==0)
			m_choiceWave = IDC_NO_WAVE;
		else if (level == 1)
			m_choiceWave = IDC_EXIST_WAVE;
	}
	int GetImageDataGrade(){ return m_choiceImage; }
	void SetImageDataLevel(int level)
	{
		if (level == 0)
			m_choiceImage = IDC_NO_IMAGE;
		else if (level == 1)
			m_choiceImage = IDC_FULL_IMAGE;
		else if (level == 2)
			m_choiceImage = IDC_QUARTER_IMAGE;
	}

// 对话框数据
	enum { IDD = IDD_SELECT_DATA_GRADE };
	
protected:
	int m_choiceWave;
	int m_choiceImage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
