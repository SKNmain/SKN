#include "binary_heap.h"

struct binary_heap
{
	int(*compare_nodes)(heap_node*, heap_node*);
	heap_node** heap;
	size_t size;
	size_t capacity;

};

heap_node* create_node(void* data)
{
	heap_node* new_node = (heap_node*)malloc(sizeof(heap_node));
	new_node->data = data;
	return new_node;
}

size_t heap_size(binary_heap* heap)
{
	return heap != NULL ? heap->size : 0;
}

size_t left(size_t i)
{
	return (i << 1) + 1;
}

size_t right(size_t i)
{
	return (i << 1) + 2;
}

size_t parent(size_t i)
{
	return i == 0 ? 0 : (i - 1) >> 1;
}

size_t index_of_last_parent(binary_heap* heap)
{
	return parent(heap->size - 1);
}

heap_node* get_child_node(binary_heap* heap, size_t i, size_t(*index_func)(size_t i))
{
	size_t index = index_func(i);
	heap_node* node;
	if (index < heap->size)
	{
		node = heap->heap[index];
	}
	else
	{
		node = NULL;
	}
	return node;
}


binary_heap* create_binary_heap(int capacity, int(*compare_nodes_func)(heap_node*, heap_node*))
{
	binary_heap* new_heap = (binary_heap*)malloc(sizeof(binary_heap));
	new_heap->size = 0;
	new_heap->capacity = capacity > 0 ? capacity : 4;
	new_heap->heap = (heap_node**)calloc(new_heap->capacity, sizeof(heap_node));
	new_heap->compare_nodes = compare_nodes_func;
	return new_heap;
}


size_t get_index_of_wrong_child(binary_heap* heap, size_t parent_index)
{
	size_t child_index = parent_index;

	//If left child greater than parent, set maxIndex = [index of left child]
	if (heap->compare_nodes(get_child_node(heap, parent_index, left), heap->heap[parent_index]) > 0)
	{
		child_index = left(parent_index);
	}

	//Check if right child is greater than maxIndex's node, and set maxIndex=[right child index]
	if (heap->compare_nodes(get_child_node(heap, parent_index, right), heap->heap[child_index]) > 0)
	{
		child_index = right(parent_index);
	}
	return child_index;
}


bool heapify(binary_heap* heap, size_t parent_index)
{
	bool swapped = false;
	//If the node has at least one child
	if (heap->size > (parent_index << 1))
	{
		size_t child_index = get_index_of_wrong_child(heap, parent_index);
		if (child_index != parent_index)
		{
			//SWAP nodes
			heap_node* temp = heap->heap[child_index];
			heap->heap[child_index] = heap->heap[parent_index];
			heap->heap[parent_index] = temp;

			//Call heapify for swapped child index's node
			heapify(heap, child_index);

			//The function returns true only if swap was happened,
			//that's mean that we should heapify the parent of current node.
			swapped = true;
		}
	}
	return swapped;
}

bool push_node(binary_heap* heap, void* data)
{
	//BIG PROBLEMS HERE can't realloc pointer from
	//another memory block, it gives the error during free.
	if (heap->size == heap->capacity)
	{
		heap->capacity <<= 1;
		heap->heap = (heap_node**)realloc(heap->heap, heap->capacity * sizeof(heap_node*));
	}

	if (heap->size < heap->capacity)
	{
		heap_node* newNode = create_node(data);

		heap->heap[(heap->size)++] = newNode;
		if (heap->size > 1)
		{
			size_t index = index_of_last_parent(heap);
			while (heapify(heap, index))
			{
				if (index == 0)
				{
					break;
				}
				else
				{
					index = parent(index);
				}
			}
		}
		return true;
	}
	return false;
}


void* pop(binary_heap* heap)
{
	void* data = NULL;
	if (heap != NULL && heap->heap != NULL & heap->size != 0)
	{
		data = heap->heap[0]->data;
		free(heap->heap[0]);
		heap->heap[0] = heap->heap[--(heap->size)];
		heap->heap[heap->size] = NULL;
		heapify(heap, 0);
	}
	return data;
}

void free_heap(binary_heap* heap, void(*node_deleter)(heap_node* node))
{
	size_t size = heap->size;

	for (size_t i = 0; i < size; ++i)
	{
		node_deleter(heap->heap[i]);
		free(heap->heap[i]);
	}
	free(heap->heap);
	free(heap);
}

bool is_binary_heap(binary_heap * heap)
{
	size_t parent_index = index_of_last_parent(heap);
	size_t child_index = parent_index;
	bool is_heap = false;
	for (; parent_index >= 0; --parent_index)
	{
		child_index = get_index_of_wrong_child(heap, parent_index);

		if (parent_index != child_index)
		{
			break;
		}

		if (parent_index == 0)
		{
			is_heap = true;
			break;
		}
	}
	return is_heap;
}

binary_heap* build_heap_from_data(const void* const data_array, const size_t array_size, const size_t element_size, int(*compare_nodes_func)(heap_node*, heap_node*))
{
	binary_heap* heap = create_binary_heap(array_size, compare_nodes_func);
	void* temp;
	for (size_t i = 0; i < array_size; ++i)
	{
		temp = memcpy(malloc(element_size), ((char*)data_array + i * element_size), element_size);
		push_node(heap, temp);
	}
	return heap;
}

void print_heap(binary_heap * heap, void(*printNode)(heap_node *node))
{
	if (heap != NULL || heap->heap != NULL)
	{
		for (size_t i = 0; i < heap->size; ++i)
		{
			printNode(heap->heap[i]);
		}
	}
}


/*
//HELPER FUNCTIONS
*/
void int_node_deleter(heap_node * node)
{
	if (node != NULL || node->data != NULL)
	{
		free(node->data);
	}
}


void print_int_node(heap_node* node)
{
	if (node != NULL || node->data != NULL)
	{
		printf(" %d ", *((int*)node->data));
	}
}

//Return > 0 if node1.data > node2.data || node2 == NULL
//Reutrn < 0 if node2.data > node1.data || node1 == NULL
int compare_int_nodes_max(heap_node* node1, heap_node* node2)
{
	if (node1 == NULL || node1->data == NULL)
	{
		return -1;
	}
	else if (node2 == NULL || node2->data == NULL)
	{
		return 1;
	}
	else
	{
		int* node1_data = (int*)node1->data;
		int* node2_data = (int*)node2->data;

		return *node1_data - *node2_data;
	}
}