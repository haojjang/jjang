// Author: Justin Fung


#include "sudoku.h"


SudokuSolver::SudokuSolver()
{} // SodukoSolver default constructor.


SudokuSolver::~SudokuSolver()
{} // SudokuSolver default destructor.


void SudokuSolver::start(char *sudoku_input)
{
	int k = 0;
	blanks = 0;
	
	solve_cnt = 0;
	int chart[9][9] = { {10, 10, 10, 11, 11, 11, 12, 12, 12},
						{10, 10, 10, 11, 11, 11, 12, 12, 12}, 
						{10, 10, 10, 11, 11, 11, 12, 12, 12},
						{13, 13, 13, 14, 14, 14, 15, 15, 15},
						{13, 13, 13, 14, 14, 14, 15, 15, 15},
						{13, 13, 13, 14, 14, 14, 15, 15, 15},
						{16, 16, 16, 17, 17, 17, 18, 18, 18},
						{16, 16, 16, 17, 17, 17, 18, 18, 18},
						{16, 16, 16, 17, 17, 17, 18, 18, 18} };
						
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			sudoku_chart[i][j] = chart[i][j];
	
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if (sudoku_input[j+k] != '.')
			{
				sudoku_table[i][j] = sudoku_input[j+k] - 48;
				sudoku_table2[i][j] = sudoku_input[j+k] - 48;
			}
			else
			{
				sudoku_table[i][j] = 0;
				sudoku_table2[i][j] = 0;
				blanks++;
			}
		}
			
		k += 9;
	}

	sudoku_guesses = new queue<int>[blanks];
	multi_guesses = new queue<int>[blanks];
	road_block = new int[blanks];
	road_block = (int*) calloc(blanks, sizeof(int));
	
	for(int i = 0; i < blanks; i++)
		for(int j = 1; j < 10; j++)
			sudoku_guesses[i].push(j);
} // start


bool SudokuSolver::solve()
{
	int row, col, k = 0, coord_cnt = 0;
	short choice, multi_choice, mode = 0;
	stack <int> choices;
	stack <int> coords;
	
	solve_cnt++;
	
	if (solve_cnt > 1)
		mode = 1;
	
	while (1)
	{
		for(int i = 0; i < 9; i++)
		{
			col = 10;
			row = 10;
		
			for(int j = 0; j < 9; j++)
				if (sudoku_table[i][j] == 0)
				{
					col = j;
					break;
				}
			
			if (col != 10)
			{
				row = i;
				break;
			}
		}

		// Save coordinates onto stack.
		if ((row != 10) || (col != 10))
		{
			coords.push(row);
			coords.push(col);
			coord_cnt = coords.size() / 2;
		}
		
		choice = check(row, col, coord_cnt, mode);
		
		if (choice == 99)
		{
			cout << "No solutions." << endl;
			return 0;
		}
		
		if (choice == 0)
		{
			col = coords.top();
			coords.pop();
			row = coords.top();
			coords.pop();
			
			if (coords.empty())
			{
				sudoku_table[row][col] = 0;
				continue;
			}
				
			col = coords.top();
			coords.pop();
			row = coords.top();
			coords.pop();
				
			sudoku_table[row][col] = 0;
			choices.pop();
		}
		else
		{
			// This code saves multi guesses for future.
			// ===================================================
			if (solve_cnt < 2)
			{
				multi_choice = check(row, col, coord_cnt);
				while (multi_choice != choice)
				{
					multi_guesses[coord_cnt-1].push(multi_choice);
					multi_choice = check(row, col, coord_cnt);
				}
			}
			// ===================================================
			
			choices.push(choice);
		
			// Write guess to sudoku table.
			sudoku_table[row][col] = choice;
		}
		
		if ( (int) choices.size() == blanks )
			break;
	}
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			solution[j+k] = sudoku_table[i][j];
		
		k += 9;
	}
	
	return 1;
} // solve


int SudokuSolver::check(int row, int col, int coord_cnt, short mode)
{
	int temp_guess = 0;
	short row_begin, row_end, col_begin, col_end, block_flag;
	short row_col_flag;

	if (mode == 0)
	{
		while (!sudoku_guesses[coord_cnt-1].empty())
		{
			temp_guess = sudoku_guesses[coord_cnt-1].front();
			sudoku_guesses[coord_cnt-1].pop();
		
			switch ( sudoku_chart[row][col] )
			{
				case 10: row_begin = 0; row_end = 3; col_begin = 0; col_end = 3; break;
				case 11: row_begin = 0; row_end = 3; col_begin = 3; col_end = 6; break;
				case 12: row_begin = 0; row_end = 3; col_begin = 6; col_end = 9; break;
				case 13: row_begin = 3; row_end = 6; col_begin = 0; col_end = 3; break;
				case 14: row_begin = 3; row_end = 6; col_begin = 3; col_end = 6; break;
				case 15: row_begin = 3; row_end = 6; col_begin = 6; col_end = 9; break;
				case 16: row_begin = 6; row_end = 9; col_begin = 0; col_end = 3; break;
				case 17: row_begin = 6; row_end = 9; col_begin = 3; col_end = 6; break;
				case 18: row_begin = 6; row_end = 9; col_begin = 6; col_end = 9; break;
			}
			
			block_flag = 0;
			row_col_flag = 0;
			for (int i = row_begin; i < row_end; i++)
			{
				for (int j = col_begin; j < col_end; j++)
					if (sudoku_table[i][j] == temp_guess)
					{
						block_flag = 1;
						break;
					}
					
				if (block_flag == 1)
					break;
			}
			
			if (block_flag == 0)
			{
				for (int i = 0; i < 9; i++)
					if ((sudoku_table[row][i] == temp_guess) || (sudoku_table[i][col] == temp_guess))
					{
						row_col_flag = 1;
						break;
					}
				
				if (row_col_flag == 0)
					return temp_guess;
			}
		}
		
		road_block[coord_cnt-1]++;
		
		if (road_block[coord_cnt-1] > 3570000)
			return 99;
		
		for(int j = 1; j < 10; j++)
			sudoku_guesses[coord_cnt-1].push(j);
	}
	else
	{
		while (!multi_guesses[coord_cnt-1].empty())
		{
			temp_guess = multi_guesses[coord_cnt-1].front();
			multi_guesses[coord_cnt-1].pop();
		
			switch ( sudoku_chart[row][col] )
			{
				case 10: row_begin = 0; row_end = 3; col_begin = 0; col_end = 3; break;
				case 11: row_begin = 0; row_end = 3; col_begin = 3; col_end = 6; break;
				case 12: row_begin = 0; row_end = 3; col_begin = 6; col_end = 9; break;
				case 13: row_begin = 3; row_end = 6; col_begin = 0; col_end = 3; break;
				case 14: row_begin = 3; row_end = 6; col_begin = 3; col_end = 6; break;
				case 15: row_begin = 3; row_end = 6; col_begin = 6; col_end = 9; break;
				case 16: row_begin = 6; row_end = 9; col_begin = 0; col_end = 3; break;
				case 17: row_begin = 6; row_end = 9; col_begin = 3; col_end = 6; break;
				case 18: row_begin = 6; row_end = 9; col_begin = 6; col_end = 9; break;
			}
			
			block_flag = 0;
			row_col_flag = 0;
			for (int i = row_begin; i < row_end; i++)
			{
				for (int j = col_begin; j < col_end; j++)
					if (sudoku_table[i][j] == temp_guess)
					{
						block_flag = 1;
						break;
					}
					
				if (block_flag == 1)
					break;
			}
			
			if (block_flag == 0)
			{
				for (int i = 0; i < 9; i++)
					if ((sudoku_table[row][i] == temp_guess) || (sudoku_table[i][col] == temp_guess))
					{
						row_col_flag = 1;
						break;
					}
				
				if (row_col_flag == 0)
					return temp_guess;
			}
		}
	}
		
	return 0;
} // check


void SudokuSolver::optimize1()
{
	int block[9], a, val, count;
	short row_begin, row_end, col_begin, col_end;
	
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (sudoku_table[i][j] == 0)
			{
				switch ( sudoku_chart[i][j] )
				{
					case 10: row_begin = 0; row_end = 3; col_begin = 0; col_end = 3; break;
					case 11: row_begin = 0; row_end = 3; col_begin = 3; col_end = 6; break;
					case 12: row_begin = 0; row_end = 3; col_begin = 6; col_end = 9; break;
					case 13: row_begin = 3; row_end = 6; col_begin = 0; col_end = 3; break;
					case 14: row_begin = 3; row_end = 6; col_begin = 3; col_end = 6; break;
					case 15: row_begin = 3; row_end = 6; col_begin = 6; col_end = 9; break;
					case 16: row_begin = 6; row_end = 9; col_begin = 0; col_end = 3; break;
					case 17: row_begin = 6; row_end = 9; col_begin = 3; col_end = 6; break;
					case 18: row_begin = 6; row_end = 9; col_begin = 6; col_end = 9; break;
				}
				
				a = 0;
				val = 0;
				count = 0;
				int mask[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				
				for (int x = row_begin; x < row_end; x++)
				{
					for (int y = col_begin; y < col_end; y++)
					{
						block[(y-col_begin)+a] = sudoku_table[x][y];
						if (block[(y-col_begin)+a] == 0)
							block[(y-col_begin)+a] = 10;
					}
					
					a += 3;
				}
				
				for (int p = 0; p < 9; p++)
					for (int q = 0; q < 9; q++)
						if (block[p] == mask[q])
							mask[q] = 20;
					
				for (int z = 0; z < 9; z++)
					for (int k = 0; k < 9; k++)
						if ((sudoku_table[i][k] == mask[z]) || (sudoku_table[k][j] == mask[z]))
						{
							block[z] = 10;
							break;
						}
							
				for (int z = 0; z < 9; z++)
					if (mask[z] < 10)
					{
						val = z;
						count++;
					}
					
				if (count == 1)
				{
					sudoku_table[i][j] = val;
					sudoku_table2[i][j] = val;
				}
			}
		}
} // optimize1


bool SudokuSolver::invalid_check()
{
	short row_begin, row_end, col_begin, col_end, count, count2, val;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (sudoku_table[i][j] != 0)
			{
				val = sudoku_table[i][j];
				
				switch ( sudoku_chart[i][j] )
				{
					case 10: row_begin = 0; row_end = 3; col_begin = 0; col_end = 3; break;
					case 11: row_begin = 0; row_end = 3; col_begin = 3; col_end = 6; break;
					case 12: row_begin = 0; row_end = 3; col_begin = 6; col_end = 9; break;
					case 13: row_begin = 3; row_end = 6; col_begin = 0; col_end = 3; break;
					case 14: row_begin = 3; row_end = 6; col_begin = 3; col_end = 6; break;
					case 15: row_begin = 3; row_end = 6; col_begin = 6; col_end = 9; break;
					case 16: row_begin = 6; row_end = 9; col_begin = 0; col_end = 3; break;
					case 17: row_begin = 6; row_end = 9; col_begin = 3; col_end = 6; break;
					case 18: row_begin = 6; row_end = 9; col_begin = 6; col_end = 9; break;
				}
				
				count = 0;
				for (int x = row_begin; x < row_end; x++)
				{
					for (int y = col_begin; y < col_end; y++)
						if (sudoku_table[x][y] == val)
						{
							count++;
							if (count >= 2)
							{
								cout << "No solutions." << endl;
								return 1;
							}
						}
				}
				
				count = 0;
				count2 = 0;
				for (int z = 0; z < 9; z++)
				{
					if (sudoku_table[i][z] == val)
					{
						count++;
						if (count >= 2)
						{
							cout << "No solutions." << endl;
							return 1;
						}
					}
					
					if (sudoku_table[z][j] == val)
					{
						count2++;
						if (count2 >= 2)
						{
							cout << "No solutions." << endl;
							return 1;
						}
					}
				}
			}
		}
	return 0;
} // valid_check


void SudokuSolver::clear()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			sudoku_table[i][j] = sudoku_table2[i][j];
} // clear


void SudokuSolver::print()
{
	int k = 0;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			solution[j+k] = sudoku_table[i][j];
			cout << sudoku_table[i][j];
		}
		
		k += 9;
	}
			
	cout << '\n';
} // print