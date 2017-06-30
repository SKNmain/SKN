#include "Huffman.h"


huff_node* create_huff_node(float freq, char sym, huff_node * l, huff_node * r)
{
	huff_node* new_node = (huff_node*)malloc(sizeof(huff_node));
	new_node->frequency = freq;
	new_node->symbol = sym;
	new_node->left = l;
	new_node->right = r;
	return new_node;
}

binary_heap* create_heap_from_huff_table(huff_table* table)
{
	binary_heap* heap = NULL;
	if (table != NULL)
	{
		heap = create_binary_heap(table->size, compare_huff_nodes_min);
		for (int i = 0; i < table->size; ++i)
		{
			push_node(
				heap,
				create_huff_node(
					table->symbol_frequencies[i],
					table->symbols[i],
					NULL,
					NULL
				)
			);
		}
	}
	return heap;
}

huff_node* create_tree_from_heap(binary_heap* heap)
{
	huff_node* root = NULL;
	if (heap != NULL)
	{
		huff_node* left = NULL;
		huff_node* right = NULL;
		do
		{
			right = (huff_node*)pop(heap);
			left = (huff_node*)pop(heap);

			push_node(
				heap,
				create_huff_node(left->frequency + right->frequency,
					0,
					left,
					right
				)
			);
		} while (heap_size(heap) > 1);
		root = (huff_node*)pop(heap);
	}
	return root;
}

huff_node* create_tree_from_table(huff_table* table)
{
	huff_node* root = NULL;
	if (table != NULL)
	{
		binary_heap* heap = create_heap_from_huff_table(table);
		root = create_tree_from_heap(heap);
	}
	return root;
}

void write_tree_data_to_table(huff_table* table, huff_node*node)
{
	if (node != NULL && table != NULL)
	{
		write_tree_data_to_table(table, node->left);
		write_tree_data_to_table(table, node->right);
		if (node->symbol != 0)
		{
			table->symbols[table->size] = node->symbol;
			table->symbol_frequencies[table->size] = node->frequency;
			++table->size;
		}
	}
}

huff_table* create_table_from_tree(huff_node* root)
{
	huff_table* table = create_huff_table_for_filing();
	write_tree_data_to_table(table, root);
	table->symbols = (char*)realloc(table->symbols, sizeof(char)*table->size);
	table->symbol_frequencies = (float*)realloc(table->symbol_frequencies, sizeof(float)*table->size);
	return table;
}

unsigned char get_code_for_symbol(huff_node* node, unsigned char *code, size_t tree_level, char symbol)
{
	unsigned char res = 0;
	if (node != NULL)
	{
		if (node->symbol == symbol)
		{
			res = tree_level;
		}
		else
		{
			//Change bit to 1 because the right edge = 1
			(*code) ^= 1 << tree_level;
			res = get_code_for_symbol(node->right, code, tree_level + 1, symbol);
			if (res == 0)
			{
				//Change bit to 0 again because there is no match in right subtree
				(*code) ^= 1 << tree_level;
				res = get_code_for_symbol(node->left, code, tree_level + 1, symbol);
			}
		}
	}
	return res;
}

char* get_string_code_for_symbol(huff_node* tree, char symbol)
{
	unsigned char code = 0;
	//unsigned char* code = (unsigned char*)calloc(1, sizeof(unsigned char));
	unsigned char length = get_code_for_symbol(tree, &code, 0, symbol);
	char* str_code = (char*)malloc((length+1) * sizeof(char));

	for (int i = 0; i < length; ++i)
	{
		str_code[i] = (code & 1 << i) ? '1' : '0';
	}
	str_code[length] = '\0';
	return str_code;
}

codes_table* get_codes(huff_node* tree, huff_table* table)
{
	codes_table* codes = NULL;
	if (tree != NULL && table != NULL)
	{
		codes = create_codes_table(table->size);

		for (int i = 0; i < table->size; ++i)
		{
			codes->length[i] = get_code_for_symbol(tree, (codes->codes + i), 0, table->symbols[i]);
		}
	}
	return codes;
}


void encode_to_file(char* data, size_t data_size, char* file_name)
{
	huff_table* table = create_huff_table_from_data(data, data_size);
	huff_node* tree = create_tree_from_table(table);


	if (tree != NULL && table != NULL && file_name != NULL && strlen(file_name))
	{
		codes_table *codes = get_codes(tree, table);

		size_t coded_data_size = 0;
		for (int i = 0; i < table->size; ++i)
		{
			coded_data_size += codes->length[i] * (table->symbol_frequencies[i]*data_size);
		}

		coded_data_size += 8 - coded_data_size % 8; //CEIL BITS TO BYTES COUNT
		coded_data_size /= 8; //GET SIZE IN BYTES

		unsigned char* coded_data = (unsigned char*)calloc(coded_data_size, sizeof(unsigned char));

		size_t coded_char_index = coded_data_size - 1;
		size_t coded_bit_index = 0;

		for (int i = 0; i < data_size; ++i)
		{
			int sym_index = 0;
			for (; sym_index < table->size; ++sym_index)
			{
				if (data[i] == table->symbols[sym_index])
					break;
			}

			unsigned char code = codes->codes[sym_index];
			for (size_t bit_of_code = 0; bit_of_code < codes->length[sym_index]; ++bit_of_code)
			{
				if (code & (1 << bit_of_code))
				{
					coded_data[coded_char_index] ^= 1 << coded_bit_index;
				}
				++coded_bit_index;

				if (coded_bit_index % 8 == 0)
				{
					--coded_char_index;
					coded_bit_index = 0;
				}
			}
		}

		FILE * file = fopen(file_name, "wb");
		if (!feof(file))
		{
			fwrite(&table->size, sizeof(float), 1, file);
			fwrite(table->symbols, sizeof(char), table->size, file);
			fwrite(table->symbol_frequencies, sizeof(float), table->size, file);
			fwrite(&data_size, sizeof(size_t), 1, file);
			fwrite(&coded_data_size, sizeof(size_t), 1, file);
			fwrite(coded_data, sizeof(unsigned char), coded_data_size, file);
			fclose(file);
		}

		free_huff_node(tree);
		free_huff_table(table);
		free_codes_table(codes);
		free(coded_data);
	}
}


char* decode_from_file(char* file_name)
{
	char* orig_data = NULL;
	FILE* file = fopen(file_name, "rb");
	if (!feof(file))
	{
		size_t table_size;
		fread(&table_size, sizeof(float), 1, file);

		huff_table* table = create_huff_table(table_size);
		fread(table->symbols, sizeof(char), table->size, file);
		fread(table->symbol_frequencies, sizeof(float), table->size, file);

		huff_node* tree = create_tree_from_table(table);

		size_t orig_data_size;
		fread(&orig_data_size, sizeof(size_t), 1, file);
		orig_data = (char*)malloc((orig_data_size + 1) * sizeof(char));


		size_t coded_data_size;
		fread(&coded_data_size, sizeof(size_t), 1, file);

		unsigned char* coded_data = (unsigned char*)malloc(coded_data_size * sizeof(unsigned char));
		fread(coded_data, sizeof(unsigned char), coded_data_size, file);


		size_t coded_char_index = coded_data_size - 1;
		size_t coded_bit_index = 0;
		size_t orig_data_index = 0;
		const huff_node* temp = tree;

		do
		{
			if (coded_data[coded_char_index] & 1 << coded_bit_index)
			{
				temp = temp->right;
			}
			else
			{
				temp = temp->left;
			}
			++coded_bit_index;
			if (coded_bit_index % 8 == 0)
			{
				coded_bit_index = 0;
				--coded_char_index;
			}

			if (temp->symbol != 0)
			{
				orig_data[orig_data_index++] = temp->symbol;
				temp = tree;
				if (coded_char_index == 0 && orig_data_index == orig_data_size)
					break; //Because the last bits is for complection so we stoped here.
			}

		} while (coded_char_index >= 0);
		orig_data[orig_data_index] = '\0';

		free_huff_node(tree);
		free_huff_table(table);
		free(coded_data);

	}
	return orig_data;
}


huff_node* create_huff_tree(char * data, size_t data_size)
{
	huff_node* root = NULL;
	huff_table* table = create_huff_table_from_data(data, data_size);
	root = create_tree_from_table(table);
	free_huff_table(table);

	return root;
}