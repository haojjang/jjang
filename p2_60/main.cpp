// Author: Justin Fung


#include "spantree.hpp"


int main(int argc, char *argv[])
{
	int num_cities, num_old_roads, next;
	string input;
	
	cin >> num_cities;
	cin >> num_old_roads;
	
	Spantree spantree(num_cities, num_old_roads);
	
	getline(cin, input);
	input.clear();
	while (getline(cin, input))
	{
		spantree.insert(input);
		input.clear();
	}
	
	next = spantree.DFS(0);

	while (next)
		next = spantree.DFS(next);
		
	spantree.output();
	
	return 0;
} // main