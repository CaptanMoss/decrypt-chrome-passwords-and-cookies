#include "../Enumeration/enumerate.h"


INT wmi_initialize(BSTR connect_point)
{

    HRESULT hr;


    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        return 0;
    }

    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(hr))
    {
        CoUninitialize();
        return 0;
    }
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hr))
    {

        CoUninitialize();
        return 0;
    }

    hr = pLoc->ConnectServer(connect_point, NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hr))
    {
        pLoc->Release();
        CoUninitialize();
        return 0;
    }


    hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (FAILED(hr)) {

        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    return 0x1;
}

IEnumWbemClassObject* exec_query(BSTR Language, BSTR Query)
{
    IEnumWbemClassObject* pEnum = 0x0;

    HRESULT result = pSvc->ExecQuery(Language,
        Query,
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnum
    );

    if (FAILED(result))
    {

        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0x0;
    }
    return pEnum;
}
