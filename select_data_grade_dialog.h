#pragma once
#include "resource.h"

// select_data_grade_dialog �Ի���
//; ��WaveDataLevel�������õ㳮���ϴ��������ݵļ���
//; 0 = ���ϴ���1 = �ϴ�
//; ��ImageDataLevel�������õ㳮���ϴ�ͼ�����ݵļ���
//; 0 = ���ϴ���1 = �ϴ�ȫͼ��2 = �ϴ�1 / 4ͼ

class SelectDataGradeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SelectDataGradeDialog)

public:
	SelectDataGradeDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SelectDataGradeDialog();

	//��ȡradio��ť
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

// �Ի�������
	enum { IDD = IDD_SELECT_DATA_GRADE };
	
protected:
	int m_choiceWave;
	int m_choiceImage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
