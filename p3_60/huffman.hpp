// Author: Justin Fung


#ifndef HUFFMAN_HPP
  #define HUFFMAN_HPP
  
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>


using namespace std;


class Symbol
{
	string code;
	unsigned char character;
	unsigned int frequency;
	bool type;
	Symbol *left;
	Symbol *right;
	
	public:
	Symbol(unsigned char cha, int frequ, bool ty, Symbol *l, Symbol *r);
	
	friend class Heap;
}; // Symbol


class Heap
{
	string table[256];
	int heapSize, capacity;
	unsigned int total, count, freq_table[256];
	unsigned char min;
	Symbol **minHeap;
	
	public:
	Heap();
	void define(unsigned int tot, unsigned int cnt, unsigned int *f_table);
	void push_symbol(Symbol *ch);
	void pop_min();
	void plant();
	void encode(Symbol *troot, string s);
	void decode();
	void traverse(Symbol *sroot);
	void priority_check(Symbol *s1, Symbol *s2, unsigned int *arr);
	unsigned char bin2dec(const char *str);
	void output();
	void debug_heap();
}; // Heap
  
#endif
