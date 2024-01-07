.PHONY: build

deps:
	mkdir -p ./build
	./scripts/openssl.sh

build:
	C=clang CXX=clang++ cmake -B ./build -G "Ninja Multi-Config" -S .
	ninja -C ./build -f build-Release.ninja
	ninja -C ./build -f build-Debug.ninja

start: build
	mkdir -p ./run
	mkdir -p ./run/tls
	cp ./res/cert.pem ./run/tls/cert.pem
	cp ./res/key.pem ./run/tls/key.pem
	cp ./build/Release/lightdb ./run/lightdb
	./scripts/start.sh

client: build
	mkdir -p ./run
	cp ./build/Release/ldc ./run/ldc
	./run/ldc

test: build
	mkdir -p ./run
	cp ./build/Debug/test ./run/test
	./run/test

debug: build
	mkdir -p ./run
	cp ./build/Debug/test ./run/test
	gdb -q ./run/test -x ./src/testing/debug.gdb

bench: build
	mkdir -p ./run
	cp ./build/Release/bench ./run/bench
	cp ./src/bench/records.csv ./run/records.csv
	./run/bench
	

clean:
	rm -rf ./build
	rm -rf ./run
	rm -rf ./cmake-build-debug
	rm -rf ./.idea
	rm -rf ./.vscode
