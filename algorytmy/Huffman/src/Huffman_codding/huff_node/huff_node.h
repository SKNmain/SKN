#pragma once
#include <stdlib.h>
#include "../../binary_heap/binary_heap.h"

struct huff_node
{
	float frequency;
	char symbol;
	huff_node* left;
	huff_node* right;
};

int compare_huff_nodes_min(heap_node* node1, heap_node* node2);
void huff_node_deleter(heap_node * node);
void free_huff_node(huff_node* node);