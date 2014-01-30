// Author: Justin Fung


#include "huffman.hpp"


Symbol::Symbol(unsigned char cha, int frequ, bool ty, Symbol *l, Symbol *r) : character(cha), frequency(frequ),
															type(ty), left(l), right(r)
{} // Symbol


Heap::Heap()
{
	min = (int) 255;

	heapSize = 0;
	capacity = 512;
	minHeap = new Symbol* [capacity+1];
	
	memset (minHeap, 0, (capacity+1) * sizeof(Symbol*));
} // Heap


void Heap::define(unsigned int tot, unsigned int cnt, unsigned int *f_table)
{
	total = tot;
	count = cnt;
	
	for (int i = 0; i < 256; i++)
		freq_table[i] = f_table[i];
} // Heap :: define


void Heap::push_symbol(Symbol *ch)
{
	int currentNode = ++heapSize;

	while (currentNode != 1 && minHeap[currentNode/2]->frequency >= ch->frequency)
	{
		if (minHeap[currentNode/2]->frequency == ch->frequency)
		{
			if ( (int)minHeap[currentNode/2]->character < (int)ch->character )
				break;
			else if ( (int)minHeap[currentNode/2]->character == (int)ch->character )
			{
				unsigned int *arr = new unsigned int [2];
				
				priority_check (minHeap[currentNode/2], ch, arr);
				
				if (arr[0] < arr[1])
					break;
			}
		}
				
		minHeap[currentNode] = minHeap[currentNode/2];
		
		currentNode /= 2;
	}

	minHeap[currentNode] = ch;
} // Heap :: push_symbol


void Heap::pop_min()
{
	int currentNode = 1;
	int child = 2;
	
	Symbol *lastC = minHeap[heapSize--];
	minHeap[heapSize + 1] = NULL;
	
	while (child <= heapSize)
	{	
		if (child < heapSize && minHeap[child]->frequency > minHeap[child+1]->frequency)
			child++;
			
		else if (child < heapSize && minHeap[child]->frequency == minHeap[child+1]->frequency)
		{
			unsigned int *arr = new unsigned int [2];
				
			priority_check (minHeap[child], minHeap[child+1], arr);
			
			if (arr[0] > arr[1])
				child++;
					
			delete [] arr;
		}
			
		if (lastC->frequency < minHeap[child]->frequency)
			break;
				
		else if (lastC->frequency == minHeap[child]->frequency)
		{
			unsigned int *arr = new unsigned int [2];
				
			priority_check (lastC, minHeap[child], arr);
				
			if (arr[0] < arr[1])
				break;
						
			delete [] arr;
		}
			
		minHeap[currentNode] = minHeap[child];
		currentNode = child;
		child *= 2;
	}
	
	minHeap[currentNode] = lastC;
} // Heap :: pop_min


void Heap::plant()
{
	while (heapSize > 1)
	{
		Symbol *first = minHeap[1];
		pop_min();
		
		Symbol *second = minHeap[1];
		pop_min();
		
		if (first->frequency < second->frequency)
		{
			Symbol *node = new Symbol( (int)0, first->frequency + second->frequency, false, second, first );
			push_symbol (node);
		}
		else if (first->frequency == second->frequency)
		{
			unsigned int *arr = new unsigned int [2];
				
			priority_check (first, second, arr);
			
			if (arr[0] > arr[1])
			{
				Symbol *node = new Symbol( (int)0, first->frequency + second->frequency, false, first, second );
				push_symbol (node);
			}
			else
			{
				Symbol *node = new Symbol( (int)0, first->frequency + second->frequency, false, second, first );
				push_symbol (node);
			}
		}
		else
		{
			Symbol *node = new Symbol( (int)0, first->frequency + second->frequency, false, first, second );
			push_symbol (node);
		}
	}
} // Heap :: plant


void Heap::encode(Symbol *troot, string s)
{
	if (troot)
	{
		if (troot->left)
			s += '0';

		encode (troot->left, s);

		if (!troot->left && !troot->right)
		{
			table [(unsigned int)troot->character] = s;
		}
		
		s.erase (s.end()-1);

		if (troot->right)
			s += '1';

		encode (troot->right, s);
	}
} // Heap :: encode


void Heap::decode()
{
	string buffer;
	unsigned char byte, mask = 1;
	unsigned int counter = 0;

	Symbol *currentNode = minHeap[1];
	
	while (1)
	{
		byte = cin.get();
		
		for (int i = 0; i < 8; i++)
		{
			if ( (byte >> i) & mask )
			{
				if (currentNode->right)
					currentNode = currentNode->right;
				
				if (!currentNode->left && !currentNode->right)
				{
					cout << currentNode->character;
					counter++;
					
					if (counter == total)
					{
						return;
					}
					
					currentNode = minHeap[1];
				}
			}
			else
			{
				if (currentNode->left)
					currentNode = currentNode->left;
				
				if (!currentNode->left && !currentNode->right)
				{
					cout << currentNode->character;
					counter++;
					
					if (counter == total)
					{
						return;
					}
					
					currentNode = minHeap[1];
				}
			}
		}
	}
} // Heap :: decode


void Heap::traverse(Symbol *sroot)
{
	if (sroot)
	{
		traverse (sroot->left);
		
		if (!sroot->left && !sroot->right)
			if ( (int)sroot->character < (int)min )
				min = sroot->character;
		
		traverse (sroot->right);
	}
} // Heap :: traverse


void Heap::priority_check(Symbol *s1, Symbol *s2, unsigned int arr[])
{			
	min = (unsigned int)255;
	traverse (s1);
	arr[0] = min;
				
	min = (unsigned int)255;
	traverse (s2);
	arr[1] = min;
} // Heap :: priority_check


unsigned char Heap::bin2dec(const char *str)
{
	unsigned char val = 0;
    
    for (int i = 7; i >= 0; i--)
        val = 2 * val + (str[i] - '0');
    return val;
} // Heap :: bin2dex


void Heap::output()
{
	string buffer;
	unsigned int mask = 255, count = 0;
	unsigned char byte1, byte2, byte3, byte4, c;

	cout << 'H' << 'U' << 'F' << 'F' << 'M' << 'A' << '3' << '\0';
	
	for (int i = 0; i < 256; i++)
	{
		if (freq_table[i] == 0)
		{
			cout << (unsigned char)0 << (unsigned char)0 << (unsigned char)0 << (unsigned char)0;
		}
		else
		{
			byte1 = freq_table[i] & mask;
			byte2 = (freq_table[i] >> 8) & mask;
			byte3 = (freq_table[i] >> 16) & mask;
			byte4 = (freq_table[i] >> 24) & mask;
			
			cout << byte1 << byte2 << byte3 << byte4;
		}
	}
	
	encode (minHeap[1], "");
	
	buffer.clear();
	while (!cin.eof())
	{
		c = cin.get();
		
		if (cin.eof())
			break;
	
		for (unsigned int j = 0; j < table [(int)c].size(); j++)
		{
			buffer += table [(int)c][j];
			count++;

			if (count == 8)
			{
				count = 0;
				cout << bin2dec (buffer.c_str());
				buffer.clear();
			}
		}
	}
	
	while (count != 0)
	{
		buffer += '0';
		count++;
		
		if (count == 8)
		{
			cout << bin2dec (buffer.c_str());
			break;
		}
	}
} // Heap :: output


void Heap::debug_heap()
{
	for (int i = 1; i < capacity+1; i++)
		if (minHeap[i])
		{
			if (minHeap[i]->type)
				cout << minHeap[i]->character << minHeap[i]->frequency << ' ';
			else
				cout << minHeap[i]->frequency << ' ';
		}
		else
			break;
			
	cout << endl << "Heap Size: " << heapSize << endl << endl;
	
	cout << "POPPING" << endl;
	
	while (heapSize)
	{
		cout << minHeap[1]->character << minHeap[1]->frequency << " | ";
		pop_min();
		
		for (int i = 1; i < 257; i++)
			if (minHeap[i])
				cout << minHeap[i]->character << minHeap[i]->frequency << ' ';
			else
				break;
			
		cout << endl;
	}
	
	cout << endl;

} // **DEBUG_HEAP**
