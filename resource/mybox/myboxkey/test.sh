echo "werwejrjsdjfdf" > encrypt.txt
openssl rsautl -encrypt -inkey pub -pubin -in encrypt.txt -out encrypt.dat

openssl rsautl -decrypt -inkey priv -in encrypt.dat -out new_encrypt.txt

