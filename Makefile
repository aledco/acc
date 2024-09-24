CPPC = g++ # clang++
CPPFLAGS = $(shell llvm-config --cxxflags --ldflags --system-libs --libs core) -IFrontEnd

DEST = acc
OFILES = $(wildcard FrontEnd/*.o) 

$(DEST): main.cpp FrontEnd
	$(CPPC) main.cpp -g $(CPPFLAGS) -o $(DEST) $(OFILES)

toy: toy.cpp
	$(CPPC) -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

FrontEnd:
	$(MAKE) -C FrontEnd

.PHONY: clean FrontEnd
clean:
	rm -f *.o $(DEST) toy
	$(MAKE) -C FrontEnd clean
