#pragma once

#include "Resource.h"

class AboutDialog : public CDialog
{
public:
  AboutDialog();

  // �Ի�������
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  // ʵ��
protected:
  DECLARE_MESSAGE_MAP()
};