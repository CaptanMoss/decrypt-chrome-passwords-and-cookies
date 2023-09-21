#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <mbstring.h>
#include <string.h>

#include "../common/cJSON.h"
#include "../common/base64.h"
#include "../Chrome/Chrome.h"

#pragma comment(lib, "Crypt32.lib")

char* read_file(const char* filename);
BYTE* get_secretKey();