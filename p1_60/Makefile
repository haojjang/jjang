# jbfung; Fung, Justin

sudsolve : main.o sudoku.o
	g++ -O3 -ansi -Wall -g -o sudsolve main.o sudoku.o
	
main.o : main.cpp sudoku.h
	g++ -O3 -ansi -Wall -g -c main.cpp
	
sudoku.o : sudoku.cpp sudoku.h
	g++ -O3 -ansi -Wall -g -c sudoku.cpp
	
clean:
	rm -f sudsolve main.o sudoku.o