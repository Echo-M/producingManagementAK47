#pragma once
#include "Resource.h"

// CInputNameKey �Ի���

class CInputNameKey : public CDialog
{
	DECLARE_DYNAMIC(CInputNameKey)

public:
	CInputNameKey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputNameKey();
	virtual void OnOK();

// �Ի�������
	enum { IDD = IDD_INPUT_NAME_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
