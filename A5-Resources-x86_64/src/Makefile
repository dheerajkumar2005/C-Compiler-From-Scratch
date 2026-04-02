CPP = g++
BISON = bison
FLEX = flex
TGT = sclp
SCAN = scanner.l
PARSE = parser.y

OBJ = Ast.o parse.tab.o Program.o scan.o $(TGT).o SemanticError.o support.o Errors.o TAC.o utils.o RTL.o Code.o RTL_Code.o

CPPSTD = --std=c++17
CFLAGS = -g -fpermissive

HEADERS = Ast.hpp Program.hpp SemanticError.hpp support.hpp Errors.hpp TAC.hpp utils.hpp RTL.hpp Code.hpp RTL_Code.hpp

$(TGT): $(OBJ)
	$(CPP) $(CPPSTD)  $(OBJ) -o $(TGT) -ly 

scan.o: scan.c $(HEADERS)
	$(CPP) $(CPPSTD)  $(CFLAGS) -c $<

parse.tab.o: parse.tab.c $(HEADERS)
	$(CPP) $(CPPSTD)  $(CFLAGS) -c  $<

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPSTD)  $(CFLAGS) -c $<

scan.c : $(SCAN) parse.tab.h
	$(FLEX) -l --yylineno -o scan.c $(SCAN)

parse.tab.c parse.tab.h : $(PARSE)
	$(BISON) -b parse -dv  $(PARSE)

clean :
	rm -f *.o *.output
	rm -f $(TGT) 
	rm -rf parse.tab.c parse.tab.h scan.c 
	rm -rf *.out *.log *.toks *.ast *.tac *.rtl exp?
