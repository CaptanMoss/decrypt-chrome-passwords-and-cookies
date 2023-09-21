#pragma once
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

static IWbemLocator* pLoc = NULL;
static IWbemServices* pSvc = NULL;

INT wmi_initialize(BSTR connect_point);
IEnumWbemClassObject* exec_query(BSTR Language, BSTR Query);

BOOL enumurate_process_list();
