cd ./thirdparty

curl -L https://github.com/openssl/openssl/releases/download/openssl-3.1.4/openssl-3.1.4.tar.gz > openssl-3.1.4.tar.gz
tar -xf openssl-3.1.4.tar.gz
rm -rf openssl-3.1.4.tar.gz

cd ./openssl-3.1.4

./Configure
make -j$(nproc)
