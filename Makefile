.PHONY: build

deps:
	mkdir -p ./build
	./scripts/openssl.sh

build:
	C=clang CXX=clang++ cmake -B ./build -G Ninja -S .
	ninja -C ./build

start: build
	mkdir -p ./run
	mkdir -p ./run/tls
	cp ./build/ldc ./run/ldc
	cp ./res/cert.pem ./run/tls/cert.pem
	cp ./res/key.pem ./run/tls/key.pem
	cp ./build/lightdb ./run/lightdb
	./scripts/start.sh

client: build
	./build/ldc

clean:
	rm -rf ./build
	rm -rf ./run
