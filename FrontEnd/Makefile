CPPC = g++

all: lexer.yy.o parser.tab.o

lexer.yy.o: lexer.yy.cpp lexer.hpp parser.tab.hpp
	$(CPPC) -c $< -o $@

parser.tab.o: parser.tab.cpp lexer.hpp
	$(CPPC) -c $< -o $@

lexer.yy.cpp: lexer.ll
	flex -o $@ $<

parser.tab.hpp parser.tab.cpp parser.output: parser.yy
	bison $< -o parser.tab.cpp

.PHONY: clean
clean:
	rm -f *.o lexer.yy.* parser.tab.*
