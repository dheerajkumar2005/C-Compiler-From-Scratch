# TODO: Update the makefile
MAIN = sclp
PARSE = parser
SCAN = scanner

$(MAIN): $(MAIN).o lex.yy.o y.tab.o AST.o
	g++ -o $(MAIN) $(MAIN).o lex.yy.o y.tab.o AST.o -ly -ll

$(PARSE): lex.yy.o y.tab.o
	gcc -o $(PARSE) lex.yy.o y.tab.o -ly -ll

$(MAIN).o: $(MAIN).cpp y.tab.h
	g++ -c $(MAIN).cpp -o $(MAIN).o

y.tab.o y.tab.h: $(PARSE).y
	yacc -dv $(PARSE).y
	gcc -c y.tab.c -o y.tab.o

lex.yy.o : $(SCAN).l y.tab.h
	lex $(SCAN).l
	gcc -c lex.yy.c -o lex.yy.o

AST.o: AST.cpp
	g++ -c AST.cpp -o AST.o

clean:
	rm -f *.o lex.yy.* *.tab.h *.tab.c y.output exp? *.out *.log $(MAIN) $(PARSE) *.toks
