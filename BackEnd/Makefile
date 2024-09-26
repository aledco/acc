CPPC ?= g++
CPPFLAGS ?=
CPPFLAGS := $(CPPFLAGS) $(shell llvm-config --cxxflags --ldflags --system-libs --libs core) -I../FrontEnd -I.

CPPFILES = $(wildcard *.cpp)
OFILES = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CPPC) $(CPPFLAGS) -c $<

all: $(OFILES)

.PHONY: clean
clean:
	rm -f *.o
