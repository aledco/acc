CPPC = g++

lexer.yy.o: lexer.yy.cpp lexer.h parser.tab.h
	$(CPPC) -c $< -o $@

parser.tab.o: parser.tab.cpp lexer.h
	$(CPPC) -c $< -o $@

lexer.yy.cpp: lexer.ll
	flex $<

parser.tab.h parser.tab.cpp parser.output: parser.yy
	bison $<

.PHONY: clean
clean:
	rm -f *.o lexer.yy.* parser.tab.*
