# jbfung; Fung, Justin

spantree : main.o spantree.o
	g++ -O3 -ansi -Wall -g -o spantree main.o spantree.o
	
main.o : main.cpp spantree.hpp
	g++ -O3 -ansi -Wall -g -c main.cpp
	
spantree.o : spantree.cpp spantree.hpp
	g++ -O3 -ansi -Wall -g -c spantree.cpp
	
clean:
	rm -f spantree main.o spantree.o