#pragma once
#include <stdlib.h>

struct huff_table
{
	char* symbols;
	float* symbol_frequencies;
	size_t size;
};

void free_huff_table(huff_table* table);
huff_table* create_huff_table(size_t size);
huff_table* create_huff_table_for_filing();
huff_table* create_huff_table_from_data(char* data, size_t data_size);

