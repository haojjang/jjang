// Author: Justin Fung


#include "sudoku.h"


bool input_check(char *s);


int main(int argc, char *argv[])
{
	int multi_flag, sols = 0;
	char sudoku_table[250];
	int sudoku_sols[50][81];
	SudokuSolver sudoku;
	
	fgets(sudoku_table, 250, stdin);
	
	if (input_check(sudoku_table))
		return 0;
		
	sudoku.start(sudoku_table);
	
	if (sudoku.invalid_check())
		return 0;
	
	sudoku.optimize1();
	
	if (sudoku.solve())
	{
		sudoku.print();

		for (int j = 0; j < 81; j++)
			sudoku_sols[sols][j] = sudoku.solution[j];

		sols++;

			while (1)
			{
				sudoku.clear();
				sudoku.optimize1();
				sudoku.solve();
				
				for (int i = 0; i < sols; i++)
				{
					multi_flag = 0;
					for (int j = 0; j < 81; j++)
					{
						if (sudoku_sols[i][j] == sudoku.solution[j])
							multi_flag++;
					}
							
					if (multi_flag == 81)
						return 0;
				}
						
				sudoku.print();
				
				for (int j = 0; j < 81; j++)
					sudoku_sols[sols][j] = sudoku.solution[j];
					
				sols++;
			}
	}
	
	return 0;
} // main


bool input_check(char *s)
{
	string garbage;
	int len = strlen(s);
	
	for (int i = 0; i < len; i++)
	{
		if (i == 80)
		{
			if (s[i+2] != '\0')
			{
				cout << "ERROR: expected \\n saw " << s[i+1] << endl;
				return 1;
			}
			
			getline(cin, garbage);
			
			if (!garbage.empty())
			{
				cout << "ERROR: expected <eof> saw " << garbage[0] << endl;
				return 1;
			}
		}
	
		if ((!(isdigit(s[i])) && !(s[i] == '.')) || (s[i] == '0'))
		{
			if (isprint(s[i]))
			{
				cout << "ERROR: expected <value> saw " << s[i] << endl;
				return 1;
			}
		}
	}
	
	if (len < 81)
	{
		cout << "ERROR: expected <value> saw \\n" << endl;
		return 1;
	}
	
	return 0;
} // input_check