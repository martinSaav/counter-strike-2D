.PHONY: all test clean clean-all editor client common server build

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

run-tests: compile-debug
	./build/taller_tests

all: clean run-tests

clean:
	cmake --build build/ --target clean

clean-all:
	rm -Rf build/
