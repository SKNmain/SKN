#pragma once
#include "codes_table\codes_table.h"
#include "huff_node\huff_node.h"
#include "huff_table\huff_table.h"

void encode_to_file(char* data, size_t data_size, char* file_name);
char* decode_from_file(char* file_name);
unsigned char get_code_for_symbol(huff_node* node, unsigned char *code, size_t tree_level, char symbol);
huff_node* create_huff_node(float freq, char sym, huff_node* left, huff_node* right);
char* get_string_code_for_symbol(huff_node* tree, char symbol);
huff_node* create_huff_tree(char* data, size_t data_size);
void free_huff_node(huff_node* node);


void huffman_to_file_test();
void huffman_to_console_test();



