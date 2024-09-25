CPPC = g++ # clang++
CPPFLAGS = $(shell llvm-config --cxxflags --ldflags --system-libs --libs core) -IFrontEnd

DEST = acc
# OFILES = $(wildcard FrontEnd/*.o)
# TEST_OFILES = $(wildcard Tests/*.o)

$(DEST): main.cpp FrontEnd
	$(CPPC) main.cpp -g $(CPPFLAGS) -o $(DEST) FrontEnd/*.o

test: Tests/main.cpp FrontEnd Tests
	echo $(wildcard Tests/*.o)
	$(CPPC) Tests/main.cpp -g -o test $(OFILES) Tests/*.o FrontEnd/*.o -lgtest

FrontEnd:
	$(MAKE) -C FrontEnd

Tests:
	$(MAKE) -C Tests

toy: toy.cpp
	$(CPPC) -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

.PHONY: clean FrontEnd Tests
clean:
	rm -f *.o $(DEST) toy test
	$(MAKE) -C FrontEnd clean
	$(MAKE) -C Tests clean
