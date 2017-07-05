#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <openssl/rsa.h>
 #include <openssl/engine.h>
 #include <openssl/pem.h>

#include<openssl/rsa.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include<string>
 #include <openssl/evp.h>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

//ERR_load_crypto_strings();
//void OpenSSL_add_all_algorithms(void);
//void OpenSSL_add_all_ciphers(void);
//void OpenSSL_add_all_digests(void);
RSA* createRSA(const char* filepath, int isPub)
{
	ifstream ifs;
	ifs.open(filepath , ios_base::in);
	string strTmp;
	string strKey = "";
	while( !ifs.eof() ) {
		getline(ifs , strTmp);
		strKey += strTmp + "\n";
	}
	ifs.close();
	strKey = strKey.substr(0,strKey.size()-1);
	cout << strKey;

	RSA *rsa= NULL;
	BIO *keybio;
	keybio = BIO_new_mem_buf((unsigned char*) strKey.c_str() , -1);
	ifs.open(filepath , ios_base::in);
	char* pwd = "1";	
	if (keybio==NULL)
	{
		printf( "Failed to create key BIO");
		return 0;
	}
	if(isPub)
	{
		PEM_read_bio_RSA_PUBKEY(keybio,&rsa,NULL,NULL);
	}
	else
	{
		PEM_read_bio_RSAPrivateKey(keybio,&rsa,NULL, NULL );
	}
	if(rsa == NULL)
	{
		printf( "Failed to create RSA");
	}
	return rsa;
}



int main(){
	const char* aa =  "123123123123";
	unsigned char buf[1024];
	unsigned char buf2[1024];
	int n=0;
	//char key[] = "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnh3cjaS9+Zg1gIWYd9Bc\nlKABffMkIkTEyZTWcsu8VLVXeSDrMS2yDFuGCVSxOl73Gcvw8SsGVDpJYUW6BmFY\nUW9x5AZdOYFxx11D7KrAVzJhJoqplRSjUaAWmUz13m5tX2OIqC1iyCZF+ZWaXrxL\nZEvqSPcDG/Qgqt/cR+j/YTu7rufzzaLXifP5fzULYJdkyVjIdMx+GJA7lXGhppRB\nXXinlRoMP82TIFSpwlRmhgoSLBDh8ayVRLqSwBpgMnafUxNF7i8/OxpIVgjlAY+c\nFgQ27FboxlwuEH19lG72jv7sXMJYvwp7RgN+rQ/x12+17fC+m6NjXpoJzPRirQwL\nFQIDAQAB\n-----END PUBLIC KEY-----";
	
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();
//	char key[] = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnh3cjaS9+Zg1gIWYd9BclKABffMkIkTEyZTWcsu8VLVXeSDrMS2yDFuGCVSxOl73Gcvw8SsGVDpJYUW6BmFY\nUW9x5AZdOYFxx11D7KrAVzJhJoqplRSjUaAWmUz13m5tX2OIqC1iyCZF+ZWaXrxL\nZEvqSPcDG/Qgqt/cR+j/YTu7rufzzaLXifP5fzULYJdkyVjIdMx+GJA7lXGhppRB\nXXinlRoMP82TIFSpwlRmhgoSLBDh8ayVRLqSwBpgMnafUxNF7i8/OxpIVgjlAY+cFgQ27FboxlwuEH19lG72jv7sXMJYvwp7RgN+rQ/x12+17fC+m6NjXpoJzPRirQwLFQIDAQAB";
	RSA * rsa = createRSA("keys/public_key.pem",1);
//	RSA * rsa = createRSA("/home/pi/test/public.pem",1);
//	RSA * rsa = createRSA("myboxkey/pub",1);
	
	cout << "===DATA==" << endl;
	cout << aa << endl;
	//n=RSA_public_encrypt(sizeof(aa)-1, (unsigned char*) aa , buf, rsa , RSA_PKCS1_PADDING);
	n=RSA_public_encrypt(RSA_size(rsa), (unsigned char*) aa , buf, rsa , RSA_PKCS1_PADDING);
	cout << "count : " << n << endl;
	buf[n] = '\0';


	cout << buf << endl;
	
	cout << "decrypt..." << endl;
//	char key2[] = "-----BEGIN PRIVATE KEY-----\nMIIBJwIBADANBgkqhkiG9w0BAQEFAASCAREwggENAgEAAoIBAQCeHdyNpL35mDWAhZh30FyUoAF98yQiRMTJlNZyy7xUtVd5IOsxLbIMW4YJVLE6XvcZy/DxKwZUOklhRboGYVhRb3HkBl05gXHHXUPsqsBXMmEmiqmVFKNRoBaZTPXebm1fY4ioLWLIJkX5=lZpevEtkS+pI9wMb9CCq39xH6P9hO7uu5/PNoteJ8/l/NQtgl2TJWMh0zH4YkDuVcaGmlEFdeKeVGgw/zZMgVKnCVGaGChIsEOHxrJVEupLAGmAydp9TE0XuLz87GkhWCOUBj5wWBDbsVujGXC4QfX2UbvaO/uxcwli/CntGA36tD/HXb7Xt8L6bo2NemgnM9GKtDAsVAgMBAAE=-----END PRIVATE KEY-----";
	RSA *rsa2 = createRSA("keys/private_key.pem" , 0);
//	RSA *rsa2 = createRSA("/home/pi/test/private.pem" , 0);
//	RSA *rsa2 = createRSA("myboxkey/priv" , 0);
	RSA_public_decrypt(RSA_size(rsa2), (unsigned char*) buf , buf2 , rsa2 , RSA_PKCS1_PADDING);
	cout << "===decrypt====" << endl;
	cout << "buf2 : "<< endl;
	cout << buf2<< endl;	


}
