.PHONY: build

build:
	C=clang CXX=clang++ cmake -B ./build -G "Ninja Multi-Config" -S .
	ninja -C ./build -f build-Release.ninja
	ninja -C ./build -f build-Debug.ninja

test: build
	./build/Debug/test

bench: build
	./build/Release/bench

debug: build
	gdb -q ./build/Debug/test -x ./test/debug.gdb

clean:
	rm -rf ./build
