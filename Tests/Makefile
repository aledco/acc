CPPC ?= g++
CPPFLAGS ?= -g
CPPFLAGS := $(CPPFLAGS) -I../FrontEnd -I../BackEnd

TEST_CPPFILES = $(wildcard *Test.cpp)
TEST_OFILES = $(TEST_CPPFILES:.cpp=.o)

.cpp.o:
	$(CPPC) $(CPPFLAGS) -c $< -lgtest

tests: $(TEST_OFILES) test_util.o

.PHONY: clean
clean:
	rm -f *.o
