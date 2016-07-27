
// application.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "application.h"
#include "login_dialog.h"
#include "main_dialog.h"
#include "gui_resources.h"
#include "working_parameters.h"
#include "device_proxy.h"
#include "device_proxy_inl.h"
#include "splash_screen.h"
#include "debug_logger.h"
#include "local_file_saver.h"
#include "input_name_key.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define APPID   _T("{673DA19F-482E-413F-8A09-46EEF44B39D6}")

// Application

BEGIN_MESSAGE_MAP(Application, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Application ����

Application::Application()
: skipUpdate_(false)
{
	// TODO:  �ڴ˴���ӹ�����룬
	// �ɹ��캯������WinMain������
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Application ����

Application theApp;


// Application ��ʼ��

BOOL Application::InitInstance()
{
  // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

  HANDLE hMutex = CreateMutex(NULL, TRUE, APPID);
  if (hMutex == NULL) {
    AfxMessageBox(IDS_PROMPT_APP_INIT_FAILED, MB_ICONERROR | MB_OK);
    return FALSE;
  }
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    // application already running.
    return FALSE;
  }

	AfxEnableControlContainer();


	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ZMVision"));

  CoInitialize(NULL);

  if (!GuiResources::GetInstance()->Init()) {
    AfxMessageBox(IDS_PROMPT_APP_INIT_FAILED, MB_OK | MB_ICONERROR);
    return FALSE;
  }

  for (int i = 1; i < __argc; i++) {
    if (_tcscmp(__targv[i], _T("-skipupdate")) == 0) {
      skipUpdate_ = true;
    }
  }

  if (WorkingParameters::GetInstance()->IsFTPInfoBlank())
  {
	  CInputNameKey inputDlg;
	  if (inputDlg.DoModal() != IDOK)
	  {
		  return FALSE;
	  }
  }

  SplashScreen splash;
  if (splash.DoModal() != IDOK) {
    return FALSE;
  }

  LoginDialog login;
  if (login.DoModal() != IDOK) {
    return FALSE;
  }

  if (!DebugLogger::GetInstance()->Init()) {
    AfxMessageBox(IDS_PROMPT_APP_INIT_FAILED, MB_OK | MB_ICONERROR);
    return FALSE;
  }

	MainDialog dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
	}


	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int Application::ExitInstance() {
  DeviceProxy::ReleaseInstance();
  LocalFileSaver::ReleaseInstance();
  WorkingParameters::ReleaseInstance();
  GuiResources::ReleaseInstance();
  DebugLogger::ReleaseInstance();
  
  CoUninitialize();

  return CWinApp::ExitInstance();
}

bool Application::Upgrade(LPCTSTR updateFilePath, LPCTSTR version) {
  HINSTANCE hInstance = AfxGetResourceHandle();
  HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDR_APP_UPDATE), _T("EXE"));
  if (hResInfo == NULL) {
    return false;
  }  
  HGLOBAL hResData = LoadResource(hInstance, hResInfo);
  if (hResData == NULL) {
    return false;
  }
  int size = SizeofResource(hInstance, hResInfo);  
  const void *data = LockResource(hResData);
  if (data == NULL) {
    return false;
  }
  TCHAR tempFilePath[MAX_PATH];
  GetTempPath(_countof(tempFilePath), tempFilePath);
  GetTempFileName(tempFilePath, _T("~"), 0, tempFilePath);
  PathRemoveExtension(tempFilePath);
  _tcscat(tempFilePath, _T(".exe"));
  TCHAR appDir[MAX_PATH];
  GetModuleFileName(NULL, appDir, _countof(appDir));
  PathRemoveFileSpec(appDir);
  try {
    CFile file(tempFilePath, CFile::modeCreate | CFile::typeBinary | CFile::modeWrite);
    file.Write(data, size);
    file.Close();
  } catch (CException *e) {
    e->Delete();
    return false;
  }
  CString parameters;
  parameters.AppendChar(_T('\"'));
  parameters.Append(updateFilePath);
  parameters.AppendChar(_T('\"'));
  parameters.AppendChar(_T(' '));
  parameters.AppendChar(_T('\"'));
  parameters.Append(appDir);
  parameters.AppendChar(_T('\"'));
  parameters.AppendChar(_T(' '));
  parameters.AppendChar(_T('\"'));
  parameters.Append(version);
  parameters.AppendChar(_T('\"'));

  HINSTANCE instance = ShellExecute(NULL, NULL, tempFilePath, parameters, NULL, SW_SHOW); 
  if ((DWORD)instance <= 32) {
    return false;
  }
  return true;
}

