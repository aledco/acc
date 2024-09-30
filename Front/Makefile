CPPC ?= clang++-18
CPPFLAGS ?=
CPPFLAGS := $(CPPFLAGS) -I../Back -I.

CPPFILES = $(wildcard *.cpp)
OFILES = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CPPC) $(CPPFLAGS) -c $<

all: $(OFILES)

.PHONY: clean
clean:
	rm -f *.o
