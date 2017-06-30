#include "huff_node.h"

int compare_huff_nodes_min(heap_node * node1, heap_node * node2)
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
		float diff = ((huff_node*)(node1->data))->frequency
			-
			((huff_node*)(node2->data))->frequency;

		if (diff == 0.0f)
			return 0;
		else if (diff < 0)
			return 1;
		else return -1;
	}
}

void huff_node_deleter(heap_node * node)
{
	if (node != NULL || node->data != NULL)
	{
		free_huff_node((huff_node*)node->data);
		free(node->data);
	}
}

void free_huff_node(huff_node* node)
{
	if (node != NULL)
	{
		free_huff_node(node->left);
		free(node->left);

		free_huff_node(node->right);
		free(node->right);
	}
}
