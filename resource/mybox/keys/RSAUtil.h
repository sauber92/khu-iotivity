#ifndef _RSA_UTIL_H_
#define _RSA_UTIL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

// I'm not using BIO for base64 encoding/decoding.  It is difficult to use.
// Using superwills' Nibble And A Half instead
// https://github.com/superwills/NibbleAndAHalf/blob/master/NibbleAndAHalf/base64.h
#include "base64.h"

// The PADDING parameter means RSA will pad your data for you
// if it is not exactly the right size
//#define PADDING RSA_PKCS1_OAEP_PADDING
#define PADDING RSA_PKCS1_PADDING
//#define PADDING RSA_NO_PADDING

RSA* loadPUBLICKeyFromString(const char* publicKeyStr);

RSA* loadPRIVATEKeyFromString(const char* privateKeyStr);
unsigned char* rsaEncrypt(RSA *pubKey, const unsigned char* str, int dataSize,
		int *resultLen) ;
unsigned char* rsaDecrypt(RSA *privKey, const unsigned char* encryptedData,
		int *resultLen) ;
// You may need to encrypt several blocks of binary data (each has a maximum size
// limited by pubKey).  You shoudn't try to encrypt more than
// RSA_LEN( pubKey ) bytes into some packet.
// returns base64( rsa encrypt( <<binary data>> ) )
// base64OfRsaEncrypted()
// base64StringOfRSAEncrypted
// rsaEncryptThenBase64
char* rsaEncryptThenBase64(RSA *pubKey, unsigned char* binaryData,	int binaryDataLen, int *outLen) ;
// rsaDecryptOfUnbase64()
// rsaDecryptBase64String()
// unbase64ThenRSADecrypt()
// rsaDecryptThisBase64()
unsigned char* rsaDecryptThisBase64(RSA *privKey, char* base64String,
		int *outLen) ;
void loadPemKeyFromFile(const char* filepath, char* buffer) ;

/*
int encrypt(int dataSize , unsigned char* src , unsigned char * dst, const char* publicKeyFilePath ){
	ERR_load_crypto_strings();
	//const char * b64_pKey = loadPemKeyFromFile(publicKeyFilePath);
	RSA *pubKey = loadPUBLICKeyFromString(b64_pKey);
	int asciiB64ELen;
	char* asciiB64E = rsaEncryptThenBase64(pubKey, src, dataSize, &asciiB64ELen);
	memcpy((void*)dst , (void*)asciiB64E , asciiB64ELen);
	free(asciiB64E);
	RSA_free(pubKey);// free the public key when you are done all your encryption
	return asciiB64ELen;
}
*/

#endif //_RSA_UTIL_H_
