// Author: Justin Fung


#include "spantree.hpp"


Spantree::Spantree(int cities, int old_roads)
{
	num_cities = cities;
	num_old_roads = old_roads;
	nation.region_cnt = 0;
	nation.total_citis = cities;
	nation.old_roadz = old_roads;
	
	visited = new bool[num_cities];
	
	for (int i = 0; i < num_cities; i++)
		visited[i] = false;
	
	
	adjCities = new City* [cities];
	nation.adjList = new City* [cities];
	for (int i = 0; i < cities; i++)
	{
		City *new_city = new City(i, 0, 0, NULL, NULL);
		adjCities[i] = new_city;
		nation.adjList[i] = adjCities[i];
	}
} // Spantree


Spantree::~Spantree()
{} // ~Spantree


void Spantree::insert(string inp)
{
	int city, adjCity, pathLen;
	char *pch, *temp;
	City *ptr, *prev = NULL, *ptr1, *prev1 = NULL;
	
	// Parsing input string.
	// ======================================================
	temp = new char [inp.length() + 1];
	strcpy (temp, inp.c_str());
	
	pch = strtok (temp, " ");
	city = atoi (pch);
	
	pch = strtok (NULL, " ");
	adjCity = atoi (pch);
	
	pch = strtok (NULL, " ");
	pathLen = atoi(pch);
	// ======================================================
	
	
	for (ptr = adjCities[city]->next; ptr && (ptr->city < adjCity); ptr = ptr->next)
		prev = ptr;
		
	if (!prev)
	{
		if (!adjCities[city]->next)
		{
			City *new_city = new City(adjCity, city, pathLen, NULL, NULL);
			adjCities[city]->next = new_city;
		}
		else
		{
			City *new_city = new City(adjCity, city, pathLen, ptr, NULL);
			adjCities[city]->next = new_city;
		}
	}
	else
	{
		City *new_city = new City(adjCity, city, pathLen, ptr, NULL);
		prev->next = new_city;
	}
	
	// ======================================================
	
	for (ptr1 = adjCities[adjCity]->next; ptr1 && (ptr1->city < city); ptr1 = ptr1->next)
		prev1 = ptr1;
		
	if (!prev1)
	{
		if (!adjCities[adjCity]->next)
		{
			City *new_city = new City(city, adjCity, pathLen, NULL, NULL);
			adjCities[adjCity]->next = new_city;
		}
		else
		{
			City *new_city = new City(city, adjCity, pathLen, ptr1, NULL);
			adjCities[adjCity]->next = new_city;
		}
	}
	else
	{
		City *new_city = new City(city, adjCity, pathLen, ptr1, NULL);
		prev1->next = new_city;
	}

} // insert


void Spantree::search(const int v)
{
	City *ptr;

	visited[v] = true;
	for (ptr = adjCities[v]; ptr; ptr = ptr->next)
		if (!visited[ptr->city])
		{
			search(ptr->city);
			city_count++;
			sprintf(buffer, "%d", ptr->city);
			regionNodes += buffer;
			regionNodes += ',';
		}
} // search


int Spantree::DFS(const int v)
{
	int next = 0;
		
	regionNodes.clear();
	nation.region_cnt++;
	city_count = 1;
	sprintf(buffer, "%d", v);
	regionNodes += buffer;
	regionNodes += ',';
	
	search(v);
	
	nation.add (regionNodes, city_count);
	
	for (int i = 0; i < num_cities; i++)
		if (!visited[i])
		{
			next = i;
			break;
		}

	return next;
} // DFS


void Spantree::print(int num_city)
{
	City *ptr;

	for (int i = 0; i < num_city; i++)
	{
		for (ptr = adjCities[i]; ptr; ptr = ptr->next)
			cout << ptr->city << '-' << ptr->len_to_parent << ' ';
			
		cout << endl;
	}
	
	cout << endl;
	
	for (int i = 0; i < num_cities; i++)
		if (!visited[i])
			cout << 0 << ' ';
		else
			cout << 1 << ' ';
			
	cout << endl;
} // print


void Spantree::print_country()
{
	Region *ptr, *ptr2;
	City *ptr1;
	
	for (ptr = nation.head; ptr; ptr = ptr->next)
	{
		for (int i = 0; i < ptr->city_cnt; i++)
		{
			for (ptr1 = ptr->regionSet[i]; ptr1; ptr1 = ptr1->next) 
				cout << ptr1->city << '-' << ptr1->len_to_parent << ' ';
				
			cout << endl;
		}
		
		cout << endl;
	}
	
	for (ptr2 = nation.head; ptr2; ptr2 = ptr2->next)
	{
		for (int i = 1; i < ptr2->heap->capacity; i++)
		{
			if (ptr2->heap->minHeap[i])
				cout << ptr2->heap->minHeap[i]->len_to_parent << ' ';
			else
				cout << 0 << ' ';
		}
			
		cout << endl;
	}
} // print_country


int Spantree::min(int i, int j)
{
	if (i < j)
		return i;
	else
		return j;
} // min


int Spantree::max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
} // max


int Spantree::min_region(City **regSet, int cit_cnt)
{
	int min_val;

	min_val = regSet[0]->city;
	for (int i = 1; i < cit_cnt; i++)
	{
		if (min_val > regSet[i]->city)
			min_val = regSet[i]->city;
	}
	
	return min_val;
} // min_region


void Spantree::listSort(City *c)
{

} // listSort


void Spantree::output()
{
	cout << "<?xml version=\"1.4\"?>" << endl;
	cout << "<country>" << endl;
	
	Region *ptr, *ptr5;
	City *ptr1, *ptr4;
	

	for (ptr5 = nation.head; ptr5; ptr5 = ptr5->next)
	{
		if (ptr5->next)
			if (ptr5->city_cnt == ptr5->next->city_cnt)
			{
				while ((min_region(ptr5->regionSet, ptr5->city_cnt) > min_region(ptr5->next->regionSet, ptr5->next->city_cnt)) &&
						(ptr5->city_cnt == ptr5->next->city_cnt))
				{	
					City *temp = ptr5->next->head;
					ptr5->next->head = ptr5->head;
					ptr5->head = temp;
					
					if (ptr5->previ)
						ptr5 = ptr5->previ;
				}
			}
	}

	
	for (ptr = nation.head; ptr; ptr = ptr->next)
	{
		for (ptr4 = ptr->head; ptr4; ptr4 = ptr4->next)
		{
			if (ptr4->next)
				if (ptr4->len_to_parent == ptr4->next->len_to_parent)
				{
					while ((min(ptr4->next->parent, ptr4->next->city) < min(ptr4->parent, ptr4->city)) && 
							(ptr4->len_to_parent == ptr4->next->len_to_parent))
					{
						int temp = ptr4->city;
						ptr4->city = ptr4->next->city;
						int temp1 = ptr4->parent;
						ptr4->parent = ptr4->next->parent;
						ptr4->next->city = temp;
						ptr4->next->parent = temp1;
						
						if (ptr4->previ)
							ptr4 = ptr4->previ;
					}
					while ((min(ptr4->next->parent, ptr4->next->city) == min(ptr4->parent, ptr4->city)) &&
							(ptr4->len_to_parent == ptr4->next->len_to_parent) &&
							(max(ptr4->next->parent, ptr4->next->city) < max(ptr4->parent, ptr4->city)))
					{
 						{
							int temp = ptr4->city;
							ptr4->city = ptr4->next->city;
							int temp1 = ptr4->parent;
							ptr4->parent = ptr4->next->parent;
							ptr4->next->city = temp;
							ptr4->next->parent = temp1;
						}
						
						if (ptr4->previ)
							ptr4 = ptr4->previ;
					}
				}
		}
	
	
	
		cout << "<region>" << endl;
		for (ptr1 = ptr->head; ptr1; ptr1 = ptr1->next)
		{
			if (ptr1->parent < ptr1->city)
			{
				cout << "<road>" << ptr1->parent << ' ' << ptr1->city 
				<< ' ' << ptr1->len_to_parent << "</road>" << endl;
			}
			else
			{
				cout << "<road>" << ptr1->city << ' ' << ptr1->parent 
				<< ' ' << ptr1->len_to_parent << "</road>" << endl;
			}
		}
		cout << "</region>" << endl;
	}
	
	cout << "</country>" << endl;
} // output


City::City()
{} // City


City::City(int cit, int par, int len, City *n, City *p) : city(cit), parent(par), len_to_parent(len), next(n), previ(p)
{} // City


City::~City()
{} // ~City


Country::Country()
{
	head = NULL;
} // Country


Country::~Country()
{} // ~Country


void Country::add(string regions, int city_count)
{		
	Region *ptr, *prev = NULL;
	// =============================================================
	City **regionSet = new City* [city_count];
	
	char *pch, *temp;
	temp = new char [regions.length() + 1];
	strcpy (temp, regions.c_str());
	pch = strtok (temp, ",");
	
	regionSet[0] = adjList[atoi(pch)];
	
	for (int i = 1; i < city_count; i++)
	{
		pch = strtok (NULL, ",");
		regionSet[i] = adjList[atoi(pch)];
	}
	delete [] temp;
	// =============================================================
	
	for (ptr = head; ptr && (ptr->city_cnt <= city_count); ptr = ptr->next)
		prev = ptr;
	
	if (!prev)
	{
		if (!head)
		{
			Region *new_region = new Region(regionSet, city_count, NULL, NULL, total_citis, old_roadz);
			head = new_region;
		}
		else
		{
			Region *new_region = new Region(regionSet, city_count, ptr, NULL, total_citis, old_roadz);
			head = new_region;
		}
	}
	else
	{
		Region *new_region = new Region(regionSet, city_count, ptr, prev, total_citis, old_roadz);
		prev->next = new_region;
	}
} // add


Region::Region(City **regions, int city_count, Region *n, Region *p, int total_cit, int roads)
{
	City *ptr;
	head = NULL;
	city_cnt = city_count;
	total_cities = total_cit;
	old_roads = roads;
	next = n;
	previ = p;
	
	regionSet = new City* [city_count];
	for (int i = 0; i < city_count; i++)
		regionSet[i] = regions[i];
		
	heap = new MinHeap(city_count, old_roads);
	
	for (int i = 0; i < city_count; i++)
		for (ptr = regionSet[i]->next; ptr; ptr = ptr->next)
			heap->push_edge(ptr);
			
	kruskal();
} // Region


Region::~Region()
{} // ~Region


void Region::push_city(City *e)
{
	City *ptr, *prev = NULL;
	
	for (ptr = head; ptr && (ptr->len_to_parent < e->len_to_parent); ptr = ptr->next)
		prev = ptr;
		
	if (!prev)
	{
		if (!head)
		{
			City *new_city = new City(e->city, e->parent, e->len_to_parent, NULL, NULL);
			tail = new_city;
			head = new_city;
		}
		else
		{
			City *new_city = new City(e->city, e->parent, e->len_to_parent, ptr, NULL);
			head = new_city;
			ptr->previ = head;
		}
	}
	else
	{
		City *new_city = new City(e->city, e->parent, e->len_to_parent, ptr, prev);
		prev->next = new_city;
		
		if (ptr)
			ptr->previ = new_city;
	}
} // push_city


void Region::kruskal()
{
	parent = new int [total_cities];
	
	for (int i = 0; i < total_cities; i++)
		parent[i] = -1;
		
	while (heap->heapSize)
	{
		if (simpleFind(heap->minHeap[1]->city) != simpleFind(heap->minHeap[1]->parent))
		{
			simpleUnion(heap->minHeap[1]->city, heap->minHeap[1]->parent);
			push_city(heap->minHeap[1]);
		}
		
		heap->pop_min();
	}
} // kruskal


void Region::simpleUnion(int i, int j)
{
	int rooti = simpleFind(i);
	int rootj = simpleFind(j);
	
	if (rooti == rootj)
		return;
	else
		parent[rooti] = rootj;
} // simpleUnion


int Region::simpleFind(int x)
{
	int r;

	for (r = x; parent[r] >= 0; r = parent[r]);
	
	while (x != r)
	{
		int s = parent[x];
		parent[x] = r;
		x = s;
	}
	
	return r;
} // simpleFind


MinHeap::MinHeap(int citis, int roadz)
{
	heapSize = 0;
	capacity = roadz * 2;
	minHeap = new City* [capacity+1];
	
	for (int i = 0; i < capacity+1; i++)
		minHeap[i] = NULL;
} // MinHeap


MinHeap::~MinHeap()
{} // ~MinHeap


void MinHeap::sU(int index)
{
	int pIndex;
	City *temp;
	
	if (index != 1)
	{
		pIndex = index/2;
		
		if (minHeap[pIndex]->len_to_parent > minHeap[index]->len_to_parent)
		{
			temp = minHeap[pIndex];
			minHeap[pIndex] = minHeap[index];
			minHeap[index] = temp;
			sU(pIndex);
		}
	}
} // sU


void MinHeap::push_edge(City *e)
{
	heapSize++;
	
	minHeap[heapSize] = e;
	sU(heapSize);
} // push_edge


void MinHeap::pop_min()
{
	int currentNode = 1;
	int child = 2;
	
	City *lastE = minHeap[heapSize--];
	
	while (child <= heapSize)
	{
		if (child < heapSize && minHeap[child]->len_to_parent > minHeap[child+1]->len_to_parent)
			child++;
			
		if (lastE->len_to_parent <= minHeap[child]->len_to_parent)
			break;
			
		minHeap[currentNode] = minHeap[child];
		currentNode = child;
		child *= 2;
	}
	
	minHeap[currentNode] = lastE;
} // delete_min