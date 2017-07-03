#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
struct heap_node
{
	void* data;
};

/*THE IMPLEMENTATION IN THE cpp for the incapsulation*/
typedef struct binary_heap binary_heap;

/*BINARY_HEAP FUNCTIONALITY*/
binary_heap* create_binary_heap(int capacity, int(*compare_nodes_func)(heap_node*, heap_node*));

bool push_node(binary_heap* heap, void* data);

void* pop(binary_heap* heap);

void print_heap(binary_heap* heap, void(*printNode)(heap_node* node));

binary_heap* build_heap_from_data(const void* const data_array, const size_t array_size, const size_t element_size, int(*compare_nodes_func)(heap_node*, heap_node*));

size_t heap_size(binary_heap* heap);

void free_heap(binary_heap* heap, void(*node_deleter)(heap_node* node));

bool is_binary_heap(binary_heap* heap);

void test_binary_heap(bool print);
/*HELPERS*/
int compare_int_nodes_max(heap_node* node1, heap_node* node2);

heap_node* create_node(void* data);

void int_node_deleter(heap_node* node);

void print_int_node(heap_node* node);

