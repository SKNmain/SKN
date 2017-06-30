#include "Huffman.h"

void huffman_to_file_test()
{
	char str[] = "KAROLEKAMOREKLEMAREKMAKOMARKAEKLERKAOMAKLERKALERKA";
	encode_to_file(str, strlen(str), "encoded.huf");
	char* data = decode_from_file("encoded.huf");
	if (strcmp(data, str) == 0)
	{
		printf("No difference between data\n");
	}
	free(data);
}

void huffman_to_console_test()
{
	char str[] = "KAROLEKAMOREKLEMAREKMAKOMARKAEKLERKAOMAKLERKALERKA";
	huff_node* tree = create_huff_tree(str, strlen(str));

	for (int i = 0; i < strlen(str); ++i)
	{
		char* str_code = get_string_code_for_symbol(tree, str[i]);
		printf("%c the code is: %s\n", str[i], str_code);
		free(str_code);
	}
	free_huff_node(tree);
	free(tree);
}