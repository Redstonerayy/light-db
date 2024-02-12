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
	cp ./build/Release/lightdb ./run/lightdb
	rm -rf ./run/lightdb.db
	./scripts/start.sh

debug: build
	mkdir -p ./run
	cp ./build/Debug/lightdb ./run/lightdb
	rm -rf ./run/lightdb.db
	./scripts/debug.sh

client: build
	mkdir -p ./run
	cp ./build/Release/ldc ./run/ldc
	./run/ldc

clean:
	rm -rf ./build
	rm -rf ./run
	rm -rf ./cmake-build-debug
	rm -rf ./.idea
	rm -rf ./.vscode
