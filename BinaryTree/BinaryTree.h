#include "stdio.h"
#include "stdlib.h"

typedef enum
{
	BINARY_TREE_OP_STATUS_PASS = 0,
	BINARY_TREE_OP_STATUS_FAIL,
	BINARY_TREE_INVALID,
} status_t;

void *binary_tree_create();	
status_t binary_tree_add(void *, int, void**);
status_t binary_tree_delete(void *, void *);
void binary_tree_preorder_traverse(void *);	
void binary_tree_inorder_traverse(void *);	
void binary_tree_postorder_traverse(void *);	
