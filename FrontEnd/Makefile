CPPC = g++

CPPFILES = $(wildcard *.cpp):
OFILES = $(CPPFILES:.c=.o)

all: $(OFILES)

Lexer.o: Lexer.cpp Lexer.hpp
	$(CPPC) -g -c Lexer.cpp -o Lexer.o

Parser.o: Parser.cpp Parser.hpp
	$(CPPC) -g -c Parser.cpp -o Parser.o

.PHONY: clean
clean:
	rm -f *.o
