sclp: *.cc *.hh sclp.l sclp.y
	bison -b y -dv sclp.y
	flex -l --ylineno -o scan.cc sclp.l
	g++ -std=c++0x -c asm.cc -o asm.o
	g++ -std=c++0x -c rtl.cc -o rtl.o
	g++ -std=c++0x -c types.cc -o types.o
	g++ -std=c++0x -c y.tab.c -o y.tab.o
	g++ -std=c++0x -c scan.cc -o scan.o
	g++ -std=c++0x -c main.cc -o main.o
	g++ -std=c++0x -c support.cc -o support.o
	g++ -std=c++0x -c ast.cc -o ast.o
	g++ -std=c++0x -c other_classes.cc -o other_classes.o
	g++ -std=c++0x -c tac.cc -o tac.o
	g++ -std=c++0x types.o scan.o main.o y.tab.o support.o asm.o rtl.o ast.o other_classes.o tac.o -o sclp -ly

clean:
	rm -f *.o *.output
	rm -f sclp
	rm -rf y.tab.c y.tab.h scan.cc
