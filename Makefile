all: clean build compile

clean:
	rm -rf bin
	rm -rf build

build: clean
	cmake -G Ninja -B build/

compile:
	cmake --build build/

.PHONY: all clean build compile
