CPPC ?= g++
CPPFLAGS ?= -g 
CPPFLAGS := $(CPPFLAGS) -I../Front -I../IR -I../Back 

TEST_CPPFILES = $(wildcard *Test.cpp)
TEST_OFILES = $(TEST_CPPFILES:.cpp=.o)

.cpp.o:
	$(CPPC) $(CPPFLAGS) -c $< -lgtest

tests: $(TEST_OFILES) TestUtils.o Environment.o

.PHONY: clean tests
clean:
	rm -f *.o
