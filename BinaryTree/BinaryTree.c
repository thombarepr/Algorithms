#include "BinaryTree.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

typedef struct node
{
	int data;
	struct node *right;
	struct node *left;
} node_t;

typedef struct binary_tree
{
	int magic;
	node_t *root;
	pthread_mutex_t mutex;
} binary_tree_t;

typedef struct list
{
	struct list *next;
	node_t *node;
} list_t;

#define BINARY_TREE_ID 	(0x5A6B)
 
/* Macros for CHECK on binary tree handler recived from user
*  Args - 1. Handler recived from user
*         2. Name for binary_tree_t pointer, the user pointer will be typecasted to
*	  3. status_t return variable name. User can chose to modify it between BEGIN
*	     and END and use it as return value.
*  Precaution - This macro lock the mutex, so following must be avoided
*  1. Function return allowed between BT_CHECK_BEGIN and BT_CHECK_END 
*  2. Attempt to lock mutex between BT_CHECK_BEGIN and BT_CHECK_END
*/
#define BT_CHECK_BEGIN(handle, bt, ret);	\
	binary_tree_t *bt = (binary_tree_t *)handle; \
	status_t ret = BT_INVALID;	\
	if (NULL != bt && BINARY_TREE_ID == bt->magic) \
	{ \
		pthread_mutex_lock(&bt->mutex); \
		ret = BT_OP_STATUS_PASS; \

#define BT_CHECK_END(bt);	\
		pthread_mutex_unlock(&bt->mutex); \
	}

void *binary_tree_create(void)
{
	binary_tree_t *bt = calloc(sizeof(binary_tree_t), 1);

	if (bt)
	{
		bt->magic = BINARY_TREE_ID;
		pthread_mutex_init(&bt->mutex, NULL);
	}
	return bt;
}

void preorder_non_recursive(node_t *node)
{
	list_t *list = NULL;
	list_t *tmp = NULL;

	while (node)
	{
		while (node)
		{
			printf("%d\t", node->data);
			tmp = calloc(sizeof(list_t), 1);
			tmp->node = node;
			tmp->next = list;
			list = tmp;
			node = node->left;
		}

		while (list)
		{
			if (NULL != list->node->right)
			{
				tmp = list;
				node = list->node->right;
				list = list->next;
				free(tmp);
				break;
			}

			tmp = list;
			list = list->next;
			free(tmp);
		}
	}
}

void preorder_recursive(node_t *node)
{
	if (NULL != node)
	{
		printf("%d\t", node->data);
		preorder_recursive(node->left);
		preorder_recursive(node->right);
	}
}

void inorder_non_recursive(node_t *node)
{
	list_t *list = NULL;
	list_t *tmp = NULL;

	while (node)
	{
		while (node)
		{
			tmp = calloc(sizeof(list_t), 1);
			tmp->node = node;
			tmp->next = list;
			list = tmp;
			node = node->left;
		}

		while (list)
		{
			printf("%d\t", list->node->data);
			if (NULL != list->node->right)
			{
				tmp = list;
				node = list->node->right;
				list = list->next;
				free(tmp);
				break;
			}

			tmp = list;
			list = list->next;
			free(tmp);
		}
	}
}

void inorder_recursive(node_t *node)
{
	if (NULL != node)
	{
		inorder_recursive(node->left);
		printf("%d\t", node->data);
		inorder_recursive(node->right);
	}
}

void postorder_non_recursive(node_t *node)
{
	list_t *traverse_list = NULL;
	list_t *tmp = NULL;

	while (NULL != node)
	{
		while (NULL != node)
		{
			tmp = calloc(sizeof(list_t), 1);
			tmp->node = node;
			tmp->next = traverse_list;
			traverse_list = tmp;
			node = node->left;
		}

		tmp = NULL;
		while ((NULL != traverse_list) && 
			((NULL == traverse_list->node->right) || 
			 (tmp && tmp->node == traverse_list->node->right)))
		{
			tmp = traverse_list;
			traverse_list = traverse_list->next;
			printf("%d\t", tmp->node->data);
			free(tmp);
		}
		
		if (NULL != traverse_list)
			node = traverse_list->node->right;
	}
}

void postorder_recursive(node_t *node)
{
	if (NULL != node)
	{
		postorder_recursive(node->left);
		postorder_recursive(node->right);
		printf("%d\t", node->data);
	}
}

void binary_tree_preorder_traverse(void *arg)
{
	BT_CHECK_BEGIN(arg, bt, ret);
	printf("Recursive - \n");
	preorder_recursive(bt->root);
	printf("\n");
	printf("Non Recursive - \n");
	preorder_non_recursive(bt->root);
	printf("\n");
	BT_CHECK_END(bt);
}

void binary_tree_inorder_traverse(void *arg)
{
	BT_CHECK_BEGIN(arg, bt, ret);
	printf("Recursive - \n");
	inorder_recursive(bt->root);
	printf("\n");
	printf("Non Recursive - \n");
	inorder_non_recursive(bt->root);
	printf("\n");
	BT_CHECK_END(bt);
}

void binary_tree_postorder_traverse(void *handle)
{
	BT_CHECK_BEGIN(handle, bt, ret);
	printf("Recursive - \n");
	postorder_recursive(bt->root);
	printf("\n");
	printf("Non Recursive - \n");
	postorder_non_recursive(bt->root);
	printf("\n");
	BT_CHECK_END(bt);
}

void binary_search_tree_add(node_t *root, node_t *node)
{
	if (root && node)
	{
		if (node->data >= root->data)
		{
			if (NULL == root->right)
				root->right = node;
			else
				binary_search_tree_add(root->right, node);
		}	
		else
		{
			if (NULL == root->left)
				root->left = node;
			else
				binary_search_tree_add(root->left, node);
		}
	}
}
 
status_t binary_tree_add(void *handle, int data, void **out)
{
	BT_CHECK_BEGIN(handle, bt, ret);
	node_t *node = calloc(sizeof(node), 1);
	if (node)
	{
		ret = BT_OP_STATUS_PASS;
		node->data = data;
		if (NULL != bt->root)
			binary_search_tree_add(bt->root, node);
		else
			bt->root = node;
		*out = node;
	}
	else
	{
		ret = BT_OP_STATUS_FAIL;
	}
	BT_CHECK_END(bt);
	return ret;
}
