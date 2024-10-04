CPPC = clang++-18
CPPFLAGS = -g $(shell llvm-config --cxxflags --ldflags --libs | sed s/-fno-exceptions//) -Wno-unused-command-line-argument
INCLUDE = -IFront -IIR -IBack

export CPPC
export CPPFLAGS

DEST = acc

$(DEST): main.cpp Front IR Back
	$(CPPC) main.cpp $(CPPFLAGS) $(INCLUDE) -o $(DEST) Front/*.o Back/*.o

test: Tests/main.cpp Front IR Back Tests
	$(CPPC) Tests/main.cpp $(CPPFLAGS) $(INCLUDE) -o test Tests/*.o Front/*.o IR/*.o Back/*.o -lgtest

Front:
	$(MAKE) -C Front

IR:
	$(MAKE) -C IR

Back:
	$(MAKE) -C Back

Tests:
	$(MAKE) -C Tests

toy: toy.cpp
	$(CPPC) -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

.PHONY: clean Front IR Back Tests
clean:
	rm -f *.o $(DEST) toy test
	$(MAKE) -C Front clean
	$(MAKE) -C IR clean
	$(MAKE) -C Back clean
	$(MAKE) -C Tests clean
