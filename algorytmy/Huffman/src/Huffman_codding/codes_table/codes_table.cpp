#include "codes_table.h"

codes_table * create_codes_table(size_t size)
{
	codes_table* codes = (codes_table*)malloc(sizeof(codes_table));
	codes->codes = (unsigned char*)calloc(size, sizeof(unsigned char));
	codes->length = (unsigned char*)calloc(size, sizeof(unsigned char));
	return codes;
}

void free_codes_table(codes_table * table)
{
	if (table != NULL)
	{
		free(table->codes);
		free(table->length);
		free(table);
	}
}
