// Author: Justin Fung


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>


using namespace std;


class City
{
	int city, parent, len_to_parent;
	City *next;
	City *previ;
	
	City();
	City(int cit, int par, int len, City *n, City *p);
	~City();
	friend class Spantree;
	friend class Region;
	friend class Country;
	friend class MinHeap;
}; // class City


class MinHeap
{
	int heapSize, capacity;
	City **minHeap;

	MinHeap(int citis, int roadz);
	~MinHeap();
	void sU(int index);
	void push_edge(City *e);
	void pop_min();
	friend class Spantree;
	friend class Region;
}; // class MinHeap


class Region
{
	City *head;
	City *tail;
	
	int city_cnt, total_cities, old_roads;
	City **regionSet;
	MinHeap *heap;
	Region *next;
	Region *previ;
	int *parent;
	
	Region(City **regions, int city_count, Region *n, Region *p, int total_cit, int roads);
	~Region();
	void push_city(City *e);
	void kruskal();
	void simpleUnion(int i, int j);
	int simpleFind(int x);
	
	friend class Country;
	friend class Spantree;
}; // class Region


class Country
{
	Region *head;
	Region *tail;
	int region_cnt, total_citis, old_roadz;
	City **adjList;
	int **adjMatrix;
	
	public:
	Country();
	~Country();
	void add(string regions, int city_count);
	
	friend class Spantree;
}; // class Country


class Spantree
{
	int num_cities, num_old_roads;
	City **adjCities;
	Country nation;
	bool *visited;
	
	string regionNodes;
	char buffer[100001];
	int city_count;
	
	friend class Region;
	friend class Country;

	public:
	Spantree(int cities, int old_roads);
	~Spantree();
	void insert(string inp);
	void search(const int v);
	int DFS(const int v);
	int min(int i, int j);
	int max(int x, int y);
	int min_region(City **regSet, int cit_cnt);
	void listSort(City *c);
	
	void print(int num_city);
	void print_country();
	void output();
}; // class Spantree