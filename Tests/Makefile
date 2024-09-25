CPPC = g++
CPPFLAGS = -I../FrontEnd

TEST_CPPFILES = $(wildcard *Test.cpp)
TEST_OFILES = $(TEST_CPPFILES:.cpp=.o)

.cpp.o:
	$(CPPC) $(CPPFLAGS) -g -c $< -lgtest

tests: $(TEST_OFILES) test_util.o

.PHONY: clean
clean:
	rm -f *.o
