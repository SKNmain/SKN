#pragma once
#include <stdlib.h>

struct codes_table
{
	unsigned char* codes;
	unsigned char* length;
};

codes_table* create_codes_table(size_t size);
void free_codes_table(codes_table* table);
