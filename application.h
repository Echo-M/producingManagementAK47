
// ccmdbg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Application: 
// �йش����ʵ�֣������ ccmdbg.cpp
//

class Application : public CWinApp {
public:
	Application();

// ��д
public:
	virtual BOOL InitInstance();	
    virtual int	 ExitInstance();

	bool IsSkipUpdate() const { return skipUpdate_; }
	bool Upgrade(LPCTSTR updateFilePath, LPCTSTR version);

private:
	bool    skipUpdate_;

	DECLARE_MESSAGE_MAP()
};

extern Application theApp;