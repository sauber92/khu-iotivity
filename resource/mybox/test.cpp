#include "RSAUtil.h"

RSA* loadPUBLICKeyFromString(const char* publicKeyStr) {
	// A BIO is an I/O abstraction (Byte I/O?)

	// BIO_new_mem_buf: Create a read-only bio buf with data
	// in string passed. -1 means string is null terminated,
	// so BIO_new_mem_buf can find the dataLen itself.
	// Since BIO_new_mem_buf will be READ ONLY, it's fine that publicKeyStr is const.
	BIO* bio = BIO_new_mem_buf((void*) publicKeyStr, -1); // -1: assume string is null terminated

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // NO NL

	// Load the RSA key from the BIO
	RSA* rsaPubKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
	if (!rsaPubKey)
		printf(
				"ERROR: Could not load PUBLIC KEY!  PEM_read_bio_RSA_PUBKEY FAILED: %s\n",
				ERR_error_string(ERR_get_error(), NULL));

	BIO_free(bio);
	return rsaPubKey;
}

RSA* loadPRIVATEKeyFromString(const char* privateKeyStr) {
	BIO *bio = BIO_new_mem_buf((void*) privateKeyStr, -1);
	//BIO_set_flags( bio, BIO_FLAGS_BASE64_NO_NL ) ; // NO NL
	RSA* rsaPrivKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);

	if (!rsaPrivKey)
		printf(
				"ERROR: Could not load PRIVATE KEY!  PEM_read_bio_RSAPrivateKey FAILED: %s\n",
				ERR_error_string(ERR_get_error(), NULL));

	BIO_free(bio);
	return rsaPrivKey;
}

unsigned char* rsaEncrypt(RSA *pubKey, const unsigned char* str, int dataSize,
		int *resultLen) {
	int rsaLen = RSA_size(pubKey);
	unsigned char* ed = (unsigned char*) malloc(rsaLen);

	// RSA_public_encrypt() returns the size of the encrypted data
	// (i.e., RSA_size(rsa)). RSA_private_decrypt()
	// returns the size of the recovered plaintext.
	*resultLen = RSA_public_encrypt(dataSize, (const unsigned char*) str, ed,
			pubKey, PADDING);
	if (*resultLen == -1)
		printf("ERROR: RSA_public_encrypt: %s\n",
				ERR_error_string(ERR_get_error(), NULL));

	return ed;
}

unsigned char* rsaDecrypt(RSA *privKey, const unsigned char* encryptedData,
		int *resultLen) {
	int rsaLen = RSA_size(privKey); // That's how many bytes the decrypted data would be

	unsigned char *decryptedBin = (unsigned char*) malloc(rsaLen);
	*resultLen = RSA_private_decrypt(RSA_size(privKey), encryptedData,
			decryptedBin, privKey, PADDING);
	if (*resultLen == -1)
		printf("ERROR: RSA_private_decrypt: %s\n",
				ERR_error_string(ERR_get_error(), NULL));

	return decryptedBin;
}

// You may need to encrypt several blocks of binary data (each has a maximum size
// limited by pubKey).  You shoudn't try to encrypt more than
// RSA_LEN( pubKey ) bytes into some packet.
// returns base64( rsa encrypt( <<binary data>> ) )
// base64OfRsaEncrypted()
// base64StringOfRSAEncrypted
// rsaEncryptThenBase64
char* rsaEncryptThenBase64(RSA *pubKey, unsigned char* binaryData,	int binaryDataLen, int *outLen) {
	int encryptedDataLen;

	// RSA encryption with public key
	unsigned char* encrypted = rsaEncrypt(pubKey, binaryData, binaryDataLen, &encryptedDataLen);

	// To base 64
	int asciiBase64EncLen;
	char* asciiBase64Enc = base64(encrypted, encryptedDataLen,	&asciiBase64EncLen);

	// Destroy the encrypted data (we are using the base64 version of it)
	free(encrypted);

	// Return the base64 version of the encrypted data
	return asciiBase64Enc;
}

// rsaDecryptOfUnbase64()
// rsaDecryptBase64String()
// unbase64ThenRSADecrypt()
// rsaDecryptThisBase64()
unsigned char* rsaDecryptThisBase64(RSA *privKey, char* base64String,
		int *outLen) {
	int encBinLen;
	unsigned char* encBin = unbase64(base64String, (int) strlen(base64String),
			&encBinLen);

	// rsaDecrypt assumes length of encBin based on privKey
	unsigned char *decryptedBin = rsaDecrypt(privKey, encBin, outLen);
	free(encBin);

	return decryptedBin;
}

void loadPemKeyFromFile(const char* filepath, char* buffer) {
	ifstream ifs;
	ifs.open(filepath, ios_base::in);
	string strTmp;
	string strKey = "";
	while (!ifs.eof()) {
		getline(ifs, strTmp);
		strKey += strTmp + "\n";
	}
	ifs.close();
	cout << " :: read :: " << endl << strKey << endl;
	//strKey = strKey.substr(0, strKey.size() - 1);
	memcpy(buffer , strKey.c_str() , strKey.size() - 1  );

}
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

unsigned char* makeAlphaString(int dataSize) {
	unsigned char* s = (unsigned char*) malloc(dataSize);
	int i;
	for (i = 0; i < dataSize; i++)
		s[i] = 65 + i;
	s[i - 1] = 0; //NULL TERMINATOR ;)
	return s;
}


int main(int argc, const char* argv[]) {
	ERR_load_crypto_strings();
	//const char * b64_pKey = loadPemKeyFromFile("keys/public_key.pem");
	//const char * b64priv_key = loadPemKeyFromFile("keys/private_key.pem");

	char b64_pKey[1024];
	char b64priv_key[1024];
	loadPemKeyFromFile("keys/public_key.pem",b64_pKey);
	loadPemKeyFromFile("keys/private_key.pem",b64priv_key);

	cout << "publickey : " << endl <<  b64_pKey << endl;

	cout << "======================================" << endl;

	cout << "privatekey : " << endl <<  b64priv_key << endl;


	// String to encrypt, INCLUDING NULL TERMINATOR:
	int dataSize = 4;// 128 for NO PADDING, __ANY SIZE UNDER 128 B__ for RSA_PKCS1_PADDING
	char* str = "1234";
	printf("\nThe original data is:\n%s\n\n", (char*) str);

	// LOAD PUBLIC KEY
	RSA *pubKey = loadPUBLICKeyFromString(b64_pKey);

	int asciiB64ELen;
	char* asciiB64E = rsaEncryptThenBase64(pubKey, (unsigned char*)str, dataSize, &asciiB64ELen);

	RSA_free(pubKey);// free the public key when you are done all your encryption

	printf(
			"Sending base64_encoded ( rsa_encrypted ( <<binary data>> ) ):\n%s\n",
			asciiB64E);
	puts(
			"<<----------------  SENDING DATA ACROSS INTERWEBS  ---------------->>");

	char* rxOverHTTP = asciiB64E;// Simulate Internet connection by a pointer reference
	printf("\nRECEIVED some base64 string:\n%s\n", rxOverHTTP);
	puts("\n * * * What could it be?");

	// Now decrypt this very string with the private key
	RSA *privKey = loadPRIVATEKeyFromString(b64priv_key);

	// Now we got the data at the server.  Time to decrypt it.
	int rBinLen;
	unsigned char* rBin = rsaDecryptThisBase64(privKey, rxOverHTTP, &rBinLen);
	printf("Decrypted %d bytes, the recovered data is:\n%.*s\n\n", rBinLen,
			rBinLen, rBin);// rBin is not necessarily NULL
	// terminated, so we only print rBinLen chrs

	RSA_free(privKey);

	bool allEq = true;
	for (int i = 0; i < dataSize; i++)
		allEq &= (str[i] == rBin[i]);

	if (allEq)
		puts("DATA TRANSFERRED INTACT!");
	else
		puts("ERROR, recovered binary does not match sent binary");
//	free(str);
	free(asciiB64E);// rxOverHTTP
	free(rBin);
	ERR_free_strings();
}
