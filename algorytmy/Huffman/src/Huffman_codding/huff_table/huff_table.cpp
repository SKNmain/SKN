#include "huff_table.h"
#include "../../binary_heap/binary_heap.h"

void free_huff_table(huff_table * table)
{
	if (table != NULL)
	{
		free(table->symbols);
		free(table->symbol_frequencies);
		free(table);
	}
}

huff_table* create_huff_table(size_t size)
{
	huff_table* table = (huff_table*)malloc(sizeof(huff_table));
	table->size = size;
	table->symbols = (char*)calloc(size, sizeof(char));
	table->symbol_frequencies = (float*)malloc(size * sizeof(float));
	return table;
}


huff_table* create_huff_table_for_filing()
{
	huff_table* table = (huff_table*)malloc(sizeof(huff_table));
	table->symbols = (char*)calloc(256, sizeof(char));
	table->symbol_frequencies = (float*)malloc(table->size * sizeof(float));
	return table;
}

huff_table* create_huff_table_from_data(char* data, size_t data_size)
{
	huff_table* table = (huff_table*)calloc(1, sizeof(huff_table));

	if (data != NULL && data_size != 0)
	{
		table->symbols = (char*)calloc(256, sizeof(char));
		int* symbol_counts = (int*)calloc(256, sizeof(int));

		for (size_t i = 0; i < data_size; ++i)
		{
			char c = data[i];
			size_t j = 0;
			for (; j < table->size; j++)
			{
				if (table->symbols[j] == c) break;
			}

			table->symbols[j] = c;
			++symbol_counts[j];

			if (j == table->size)
			{
				++table->size;
			}
		}
		table->symbols = (char*)realloc(table->symbols, table->size * sizeof(char));
		table->symbol_frequencies = (float*)malloc(table->size * sizeof(float));
		for (size_t i = 0; i < table->size; ++i)
		{
			table->symbol_frequencies[i] = symbol_counts[i] / (float)data_size;
		}
		free(symbol_counts);
	}
	return table;
}

