CPPC = g++ # clang++
CPPFLAGS = $(shell llvm-config --cxxflags --ldflags --system-libs --libs core)

DEST = acc

$(DEST): main.cpp
	$(CPPC) -g -O3 main.cpp $(CPPFLAGS) -o $(DEST)

toy: toy.cpp
	$(CPPC) -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

.PHONY: clean
clean:
	rm -f *.o $(DEST) toy
