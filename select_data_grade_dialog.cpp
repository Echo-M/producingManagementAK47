// select_data_grade_dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "select_data_grade_dialog.h"
#include "afxdialogex.h"


// select_data_grade_dialog 对话框

IMPLEMENT_DYNAMIC(SelectDataGradeDialog, CDialogEx)

SelectDataGradeDialog::SelectDataGradeDialog(CWnd* pParent /*=NULL*/)
: CDialogEx(SelectDataGradeDialog::IDD, pParent)
{

}

SelectDataGradeDialog::~SelectDataGradeDialog()
{
}

void SelectDataGradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void SelectDataGradeDialog::OnOK()
{
	if (((CButton*)GetDlgItem(IDC_NO_WAVE))->GetCheck())
		m_choiceWave = IDC_NO_WAVE;
	else
	if (((CButton*)GetDlgItem(IDC_EXIST_WAVE))->GetCheck())
		m_choiceWave = IDC_EXIST_WAVE;
	else
		m_choiceWave = 0;
	if (((CButton*)GetDlgItem(IDC_NO_IMAGE))->GetCheck())
		m_choiceImage = IDC_NO_IMAGE;
	else
	if (((CButton*)GetDlgItem(IDC_QUARTER_IMAGE))->GetCheck())
		m_choiceImage = IDC_QUARTER_IMAGE;
	else
	if (((CButton*)GetDlgItem(IDC_FULL_IMAGE))->GetCheck())
		m_choiceImage = IDC_FULL_IMAGE;

	UpdateData(false);//更新内容
	CDialogEx::OnOK();
}

BOOL SelectDataGradeDialog::OnInitDialog()
{
    // TODO: Add your command handler code here  
    CDialog::OnInitDialog();
	((CButton*)GetDlgItem(m_choiceWave))->SetCheck(1);
	((CButton*)GetDlgItem(m_choiceImage))->SetCheck(1);
	return TRUE;
}


BEGIN_MESSAGE_MAP(SelectDataGradeDialog, CDialogEx)
END_MESSAGE_MAP()


// SelectDataGradeDialog 消息处理程序
