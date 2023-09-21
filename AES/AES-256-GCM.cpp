#include "AES-256-GCM.h"

char* aes_gcm_decrypt(unsigned char* pkey, unsigned char* piv, unsigned char* pencrypted_password, int enc_len)
{

    AutoSeededRandomPool prng;

    byte key[KEY_LENGTH];
    byte iv[TAG_SIZE];

    memcpy(key, pkey, KEY_LENGTH);
    memcpy(iv, piv, TAG_SIZE);

    string encoded;
    std::string cipher(reinterpret_cast<char*>(pencrypted_password), enc_len);


    string rpdata;

    encoded.clear();
    StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded))); 

    encoded.clear();
    StringSource(iv, sizeof(iv), true, new HexEncoder(new StringSink(encoded))); 

    encoded.clear();

    StringSource(cipher, true, new HexEncoder(new StringSink(encoded))); 


    try
    {
        GCM< AES >::Decryption d;
        d.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

        AuthenticatedDecryptionFilter df(d, new StringSink(rpdata), AuthenticatedDecryptionFilter::DEFAULT_FLAGS, TAG_SIZE); 

        StringSource(cipher, true, new Redirector(df)); 

        bool b = df.GetLastResult();
        
        assert(true == b);


    }
    catch (CryptoPP::HashVerificationFilter::HashVerificationFailed& e)
    {


    }
    catch (CryptoPP::InvalidArgument& e)
    {


    }
    catch (CryptoPP::Exception& e)
    {


    }

    char decrypt_aes_key[MAX_PATH] = "";
    SecureZeroMemory(decrypt_aes_key, MAX_PATH);

    std::copy(rpdata.begin(), rpdata.end(), decrypt_aes_key);
    decrypt_aes_key[rpdata.size()] = '\0';

    rpdata.clear();
    
    return decrypt_aes_key;
}
