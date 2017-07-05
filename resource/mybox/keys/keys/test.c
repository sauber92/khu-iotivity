#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>




void main( )

{

      

      RSA *pri_key = NULL; //서명자 개인키

      RSA *pub_key = NULL; //서명자 공개키

      EVP_PKEY *e_pub_key = NULL; //서명자 인증서

      FILE *key_f = NULL; //서명자 개인키 파일 포인터

      FILE *crt_f = NULL; // 서명자 인증서 파일 포인터 




      int sign_len; // 사인값 길이(개인키 암호화된 결과값 길이)

      unsigned char sign[256]; //사인값이 담길 버퍼(개인키로 암호화된 결과값이 담길 버퍼)

      int d_sign_len; // 사인값 복호화 길이

      unsigned char d_sign[256]; // 사인값 복호화된 값이 담길 버퍼



   EVP_PKEY *PEM_read_PUBKEY(FILE *fp, EVP_PKEY **x,
                                       NULL,NULL);  //








  




      /* 사인값 생성을 위한 사용자 개인키 로드 시작 */

      key_f = fopen("user.key", "r"); // 개인키 파일 오픈

      if( key_f == NULL ) {

            printf("File Open user.key error\n");

            return 0;

      }






      pri_key = PEM_read_RSAPrivateKey(key_f, NULL, NULL, NULL); // 파일로부터 RSA용 개인키를 로드한다.

      if( pri_key == NULL) {

            printf("Read Private Key for RSA Error\n");

            goto shutdown;

      }




      memset(sign, 0x00, sizeof(sign) ); //버퍼 초기화




      //로드한 RSA용 개인키를 이용하여 해시값을 서명한다.

      sign_len = RSA_private_encrypt( sizeof(sha256_str), sha256_str, sign, pri_key, RSA_PKCS1_PADDING);

      /*

      리턴값 : 서명값 길이

      입력값(순서대로) : 암호화할 입력값(=해시값)길이, 암호화할 입력값, 암호화한 결과가 담길 버퍼, 개인키, PADDING 유무

      Padding과 관련된 이슈는 PKCS #1 표준 참고

      */




      if( sign_len < 1 ) {

            printf("rsa private encrypt error\n");

            goto shutdown;

      }




      /* 사인값 검증을 위한 사용자 인증서 로드 시작 */

      crt_f = fopen("user.crt", "r"); //사용자 인증서 파일 오픈

      if( crt_f == NULL ) {

            printf("fopen user.crt error\n");

            goto shutdown;

      }           







      X509* user_x509 = NULL;

      user_x509 = PEM_read_X509(crt_f, NULL, NULL, NULL); //사용자 인증서 파일로부터 x509 타입 데이터 읽기

      /* 

      설명 : x509는 인증서 포맷으로 현재는 ITU-T 표준기구에서 표준화한 것으로 현재 대부분의 인증서는 x509v3 기반의 인증서를 사용하고 있다.

      */

      if( user_x509 == NULL ) {

            printf("PEM read x509 form user.crt error\n");

            goto shutdown;

      }




      e_pub_key = X509_get_pubkey(user_x509); // 사용자 x509v3 타입의 인증서로부터 공개키를 추출한다.

      if( e_pub_key == NULL ) {

            printf("get public key from input X509 error\n");

            goto shutdown;

      }




      pub_key = EVP_PKEY_get1_RSA(e_pub_key);// 추출한 공개키로부터 RSA용 공개키를 가져온다.

      if (pub_key == NULL) {

            printf("get rsa from public key error\n");

            goto shutdown;

      }




      memset(d_sign, 0x00, sizeof(d_sign) );

      d_sign_len = RSA_public_decrypt(sign_len, sign, d_sign, pub_key, RSA_PKCS1_PADDING); // 사인값을 사용자의 공개키로 복호화 한다.

      if(d_sign_len < 1) {

            printf("RSA public decrypt error\n");

            goto shutdown;

      }




      int result;




      result = memcmp( d_sign, sha256_str, d_sign_len); //복호화한 값이 메시지 해시값과 같은지 확인한다.

      if( !result ) printf("OK!\n");

      else printf("Fail!\n");

}
