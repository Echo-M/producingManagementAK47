#pragma once
#define _CRT_SECURE_NO_DEPRECATE      

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <iphlpapi.h>


typedef DWORD(CALLBACK * PGAINFO)(PIP_ADAPTER_INFO, PULONG);//GetAdaptersInfo

bool GetTerminalID(TCHAR strID[200]);
bool GetMACadress(TCHAR ptrMACAdress[14]);  //��ȡMAC��ַ  ��ʽ:xxxxxxxxxxxx
bool GetCPU_ID(TCHAR CPU_ID[18]);    //��ȡCPU ID ��ʽ��xxxxxxxx-xxxxxxxx
bool GetHdiskID(TCHAR DiskID[10]);  //��ȡӲ���߼�ID  ��ʽ:xxxxxxxx

