CPPC = g++

CPPFILES = $(wildcard *.cpp)
OFILES = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CPPC) -g -c $<

all: $(OFILES)

.PHONY: clean
clean:
	rm -f *.o
