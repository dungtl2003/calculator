all: clean build compile

clean:
	rm -rf bin
	rm -rf build

build: clean
	cmake -G Ninja -B build/

compile:
	cmake --build build/

release:
	cmake -B build/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -G Ninja
	cmake --build build/
	sudo cmake --install build/

.PHONY: all clean build compile release
