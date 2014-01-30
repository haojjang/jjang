// Author: Justin Fung


#include "huffman.hpp"


int main (int argc, char *argv[])
{
	Heap min_heap;
	string input_buffer;
	char decompress_mode[3] = "-d";
	unsigned char c;
	unsigned int freq_table[256] = {0};
	unsigned int total_freq = 0, symbol_cnt = 0;

	if (argc == 2)
	{
		if ( !strcmp(argv[1], decompress_mode) )
		{
// Begin decompression.
// ===========================================================================
			c = cin.get();
			
			while (c != '\0')
				c = cin.get();
			
			unsigned int byte1, byte2, byte3, byte4;	
			unsigned int dfreq = 0;
			
			for (int i = 0; i < 256; i++)
			{
				dfreq = 0;
				byte1 = 0;
				byte2 = 0;
				byte3 = 0;
				byte4 = 0;

				byte1 = cin.get();

				byte2 = cin.get();
				byte2 = byte2 << 8;

				byte3 = cin.get();
				byte3 = byte3 << 16;

				byte4 = cin.get();
				byte4 = byte4 >> 24;

				dfreq = byte1 + byte2 + byte3 + byte4;
				
				freq_table[i] = dfreq;
				
				if (freq_table[i] > 0)
				{
					symbol_cnt++;
					total_freq += freq_table[i];
					
					Symbol *node = new Symbol( (unsigned char)i, freq_table[i], true, NULL, NULL );
					min_heap.push_symbol(node);
				}
			}
			
			min_heap.define (total_freq, symbol_cnt, freq_table);
			min_heap.plant();
			min_heap.decode();
// ===========================================================================
		}
	}
	else
	{
// Begin compression.
// ===========================================================================	
		while (!cin.eof())
		{
			c = cin.get();
			
			if (cin.eof())
				break;
				
			freq_table[ (unsigned int)c ]++;
		}
		
		cin.clear();
		cin.seekg (std::ios::beg);
		
		for (int i = 0; i < 256; i++)
		{
			if (freq_table[i] > 0)
			{
				symbol_cnt++;
				total_freq += freq_table[i];
				
				Symbol *node = new Symbol( (unsigned char)i, freq_table[i], true, NULL, NULL );
				min_heap.push_symbol(node);
			}
		}
		
		min_heap.define (total_freq, symbol_cnt, freq_table);
		
		min_heap.plant();
		min_heap.output();
// ===========================================================================		
	}
	
	return 0;
} // main
