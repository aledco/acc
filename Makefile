# CPPC = g++
CPPC = clang++-18
CPPFLAGS = -g $(shell llvm-config --cxxflags --ldflags --libs) -Wno-unused-command-line-argument
INCLUDE = -IFrontEnd -IBackEnd

export CPPC
export CPPFLAGS

DEST = acc

$(DEST): main.cpp FrontEnd BackEnd
	$(CPPC) main.cpp $(CPPFLAGS) $(INCLUDE) -o $(DEST) FrontEnd/*.o BackEnd/*.o

test: Tests/main.cpp FrontEnd BackEnd Tests
	$(CPPC) Tests/main.cpp $(CPPFLAGS) $(INCLUDE) -o test Tests/*.o FrontEnd/*.o BackEnd/*.o -lgtest

FrontEnd:
	$(MAKE) -C FrontEnd

BackEnd:
	$(MAKE) -C BackEnd

Tests:
	$(MAKE) -C Tests

toy: toy.cpp
	$(CPPC) -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

.PHONY: clean FrontEnd BackEnd Tests
clean:
	rm -f *.o $(DEST) toy test
	$(MAKE) -C FrontEnd clean
	$(MAKE) -C BackEnd clean
	$(MAKE) -C Tests clean
