#include "BinaryTree.h"

int main()
{
	void *bt = binary_tree_create();
	void *node[30] = {NULL};
	if (bt)
	{
		int i = 0;
		while (i < 30)
		{
			binary_tree_add(bt, random()%2000, &node[i]);
			i++;
		}
		printf("Preorder:\n");
		binary_tree_preorder_traverse(bt);	
		printf("Inorder:\n");
		binary_tree_inorder_traverse(bt);	
		printf("Postorder:\n");
		binary_tree_postorder_traverse(bt);	
	}
	return 0;
}
