#include "binary_heap.h"

void test_binary_heap(bool print)
{
	size_t size = 20;
	binary_heap* heap;
	heap = create_binary_heap(10, compare_int_nodes_max);

	for (int i = 0; i < size; i++)
	{
		int* data = new int(rand() % 200); //please run srand() before function call, for realy random numbers
		if (!push_node(heap, data)) break;
		if (print)
		{
			print_heap(heap, print_int_node);
			printf("\n");
		}
	}

	if (is_binary_heap(heap))
		printf("YEAH it is binary heap.\n");

	free_heap(heap, int_node_deleter);
}