// Author: Justin Fung


#ifndef SUDOKU_H
  #define SUDOKU_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <ctype.h>
#include <stack>
#include <queue>
#include <vector>


using namespace std;


class SudokuSolver
{
	int blanks, solve_cnt;
	int sudoku_table[9][9];
	int sudoku_table2[9][9];
	int sudoku_chart[9][9];
	int *road_block;
	queue <int> *sudoku_guesses;
	queue <int> *multi_guesses;
	
	int check(int row, int col, int coord_cnt);
	
	public:
	int solution[81];
	
	SudokuSolver();
	~SudokuSolver();
	void start(char *sudoku_input);
	bool solve();
	bool invalid_check();
	void optimize1();
	void clear();
	void print();
}; // sudokuSolver

#endif