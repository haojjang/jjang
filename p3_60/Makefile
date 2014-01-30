# jbfung; Fung, Justin

huffman : main.o huffman.o
	g++ -O3 -ansi -Wall -g -o huffman main.o huffman.o
	
main.o : main.cpp huffman.hpp
	g++ -O3 -ansi -Wall -g -c main.cpp
	
huffman.o : huffman.cpp huffman.hpp
	g++ -O3 -ansi -Wall -g -c huffman.cpp
	
clean:
	rm -f huffman main.o huffman.o
