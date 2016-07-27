// input_name_key.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "input_name_key.h"
#include "afxdialogex.h"
#include "working_parameters.h"

// CInputNameKey �Ի���

IMPLEMENT_DYNAMIC(CInputNameKey, CDialog)

CInputNameKey::CInputNameKey(CWnd* pParent /*=NULL*/)
	: CDialog(CInputNameKey::IDD, pParent)
{
}

CInputNameKey::~CInputNameKey()
{
}

void CInputNameKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputNameKey, CDialog)
END_MESSAGE_MAP()


// CInputNameKey ��Ϣ�������
void CInputNameKey::OnOK()
{
	CString userName;
	CString password;
	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(userName);
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(password);

	if (!WorkingParameters::GetInstance()->SetFTPInfo(userName, password))
	{
		AfxMessageBox(_T("�˺��������������ԣ�"), MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}