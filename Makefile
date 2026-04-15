CMAKE_INSTALL_PREFIX ?= /usr

QT_PREFIX    ?= 
CXX_COMPILER ?= 
C_COMPILER   ?=
GENERATOR    ?= Ninja

CMAKE_ARGS := -G $(GENERATOR) -B build/ -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)
ifneq ($(QT_PREFIX),)
    CMAKE_ARGS += -DCMAKE_PREFIX_PATH=$(QT_PREFIX)
endif
ifneq ($(CXX_COMPILER),)
    CMAKE_ARGS += -DCMAKE_CXX_COMPILER=$(CXX_COMPILER)
endif
ifneq ($(C_COMPILER),)
    CMAKE_ARGS += -DCMAKE_C_COMPILER=$(C_COMPILER)
endif

all: clean build compile

clean:
	rm -rf bin
	rm -rf build

build: clean
	cmake $(CMAKE_ARGS)

compile:
	cmake --build build/

release: clean
	cmake $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Release
	cmake --build build/
	sudo cmake --install build/

uninstall:
	@if [ -f build/install_manifest.txt ]; then \
		sudo ./scripts/uninstall.sh; \
	else \
		echo "Manifest not found. Nothing to uninstall."; \
	fi

.PHONY: all clean build compile release uninstall
