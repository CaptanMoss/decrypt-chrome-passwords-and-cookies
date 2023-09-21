#pragma once

#include <iostream>
#include <Windows.h>

using std::cout;
using std::cerr;

#include <string>
using std::string;

#include "../crypto_headers/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "../crypto_headers/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "../crypto_headers/cryptlib.h"
using CryptoPP::BufferedTransformation;
using CryptoPP::AuthenticatedSymmetricCipher;

#include "../crypto_headers/filters.h"
using CryptoPP::Redirector;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;

#include "../crypto_headers/aes.h"
using CryptoPP::AES;

#include "../crypto_headers/gcm.h"
using CryptoPP::GCM;

#include "assert.h"

#define KEY_LENGTH 32
#define TAG_SIZE 12

using namespace std;

char* aes_gcm_decrypt(unsigned char* pkey, unsigned char* piv, unsigned char* pencrypted_password, int enc_len);