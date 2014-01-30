# Author: Justin Fung

funix.out: directory.o funix.o main.o permissions.o Time.o file.o
	g++ -Wall -g -ansi -o funix.out directory.o funix.o main.o permissions.o Time.o file.o

directory.o: directory.cpp directory.h Time.h permissions.h linkedlist.h linkedlist.cpp
	g++ -Wall -g -ansi -c directory.cpp

funix.o: funix.cpp funix.h directory.h 
	g++ -Wall -g -ansi -c funix.cpp

main.o: main.cpp funix.h 
	g++ -Wall -g -ansi -c main.cpp

permissions.o: permissions.cpp permissions.h 
	g++ -Wall -g -ansi -c permissions.cpp

Time.o: Time.cpp Time.h 
	g++ -Wall -g -ansi -c Time.cpp

file.o: file.cpp file.h
	g++ -Wall -g -ansi -c file.cpp

clean:
	rm -f funix.out directory.o funix.o main.o permissions.o Time.o file.o
