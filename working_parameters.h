#pragma once

#include <vector>
#include "singleton.h"

// class ParameterBlock
class ParameterBlock 
{
public:
  const static ParameterBlock Null;

  ParameterBlock();
  ParameterBlock(IXMLDOMElement *e); 
  ParameterBlock(const ParameterBlock &other);
  //  析构函数是虚函数
  virtual ~ParameterBlock(); 

  void Reset(IXMLDOMElement *e);
  ParameterBlock &operator=(const ParameterBlock &other);

  CString GetName() const;
  int GetIntParameter(LPCTSTR name, int defaultValue);
  double GetFloatParameter(LPCTSTR name, double defaultValue);
  CString GetStringParameter(LPCTSTR name, LPCTSTR defaultValue);
  ParameterBlock SelectBlock(LPCTSTR path);
  ParameterBlock GetFirstChildBlock();
  ParameterBlock GetNextBlock();
  bool IsNull() const;
    
private:  
  IXMLDOMElement  *e_;//IXMLDOMElement：常用的DOM接口之一。元素接口，一般用来表示一个节点及其属性。
};

// 可变配置 lux（.ini）
class VariableConfigBlock 
{
public:
	const static VariableConfigBlock Null;

	VariableConfigBlock();
	VariableConfigBlock(LPCTSTR path);
	VariableConfigBlock(const VariableConfigBlock &other);
	virtual ~VariableConfigBlock();
	VariableConfigBlock &operator=(const VariableConfigBlock &other);
	
	// 返回当前的运行模式
	CString GetCurrentMode();
	bool SetCurrentMode(LPCTSTR _currentMode);
	// 读取当前运行模式下 键名为name 的值，若该值不存在，则创建一个该键名，默认值为defauValue
	int GetIntParameter(LPCTSTR name, int defaultValue);
	bool SetIntParameter(LPCTSTR name, int _value);
	CString GetStringParameter(LPCTSTR name, LPCTSTR defaultValue);
	bool SetStringParameter(LPCTSTR name, LPCTSTR _value);
	// 检查配置文件是否存在
	bool IsNull() const;

private:
	CString configPath_;// 存储ini文件的绝对路径
};

// class WorkingParameters
class WorkingParameters : public ParameterBlock 
{
public:
  bool LoadFromServer();
  bool LoadConfigFromServer();
  bool LoadXML();
  bool LoadConfig();

  bool SetCurrentModel(int id);
  ParameterBlock *GetCurrentModelParameters();
  VariableConfigBlock *GetCurrentConfigParameters();

  int Login(LPCTSTR userName, LPCTSTR password);
  const CString &GetUserName() { return userName_; }

  bool IsAppUpdated() const { return appUpdated_; }
  const CString &GetAppUpdateFilePath() const { return appUpdateFilePath_; }
  const CString &GetLastAppVersion() const { return lastAppVersion_; }

  void SetOffline(bool offline) { offline_ = offline; }
  bool IsOffline() const { return offline_; }

  int GetCurrentModelId() const { return currentModelId_; }
  int GetCurrentPermission() const { return currentPermission_; }

  bool IsFTPInfoBlank();
  bool SetFTPInfo(LPCTSTR userName, LPCTSTR password);

private:
  CComPtr<IXMLDOMDocument2> xmlDoc_;
  int                       currentModelId_;
  ParameterBlock            currentModelParameters_;
  VariableConfigBlock		currentConfigParameters_;
  CString                   userName_;
  bool                      appUpdated_;
  CString                   appUpdateFilePath_;
  CString                   lastAppVersion_;
  bool                      offline_;
  int						currentPermission_;
  
  // 构造函数是私有的（单例模式）
  WorkingParameters();
  virtual ~WorkingParameters();

  DECLARE_SINGLETON(WorkingParameters)
};