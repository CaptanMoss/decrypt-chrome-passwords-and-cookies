#include "../Enumeration/enumerate.h"

BOOL enumurate_process_list()
{

    IEnumWbemClassObject* enumerator = 0;
    IWbemClassObject* object = 0;

    enumerator = exec_query(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Process"));


    ULONG uReturn = 0;
    int i = 0;

    while (enumerator)
    {
        i++;
        HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &object, &uReturn);
        if (uReturn == 0)
        {
            break;
        }

        VARIANT vtName;
        object->Get(L"Name", 0, &vtName, nullptr, nullptr);

        if (SUCCEEDED(hr))
        {
            VariantClear(&vtName);
        }

        char* dName = _com_util::ConvertBSTRToString(vtName.bstrVal);
        //compare process name

        if (!strcmp((const char*)dName, (const char*)"chrome.exe"))
            return 0x1;

        object->Release();
    }

    enumerator->Release();

    return 0x0;
}