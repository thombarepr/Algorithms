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
				list_t *tmp = list;
				node = list->node->right;
				list = list->next;
				free(tmp);
				break;
			}
			list = list->next;
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
				list_t *tmp = list;
				node = list->node->right;
				list = list->next;
				free(tmp);
				break;
			}
			list = list->next;
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
	list_t *print_list = NULL;
	list_t *print_list_end = NULL;
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

		while ((NULL != traverse_list) && 
			((NULL == traverse_list->node->right) || 
			 (print_list_end && print_list_end->node == traverse_list->node->right)))
		{
			tmp = traverse_list;
			traverse_list = traverse_list->next;

			if (NULL != print_list_end)
			{
				print_list_end->next = tmp;
				print_list_end = tmp;
			}
			else
			{
				print_list = tmp;
				print_list_end = tmp;
			}
		}
		
		if (NULL != traverse_list)
			node = traverse_list->node->right;
	}

	while (NULL != print_list)
	{
		tmp = print_list;
		print_list = print_list->next;
		printf("%d\t", tmp->node->data);
		free(tmp);
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
	binary_tree_t *bt = (binary_tree_t *)arg;
	if (NULL != bt && BINARY_TREE_ID == bt->magic)
	{
		pthread_mutex_lock(&bt->mutex);
		printf("Recursive - \n");
		preorder_recursive(bt->root);
		printf("\n");
		printf("Non Recursive - \n");
		preorder_non_recursive(bt->root);
		printf("\n");
		pthread_mutex_unlock(&bt->mutex);
	}
}

void binary_tree_inorder_traverse(void *arg)
{
	binary_tree_t *bt = (binary_tree_t *)arg;
	if (NULL != bt && BINARY_TREE_ID == bt->magic)
	{
		pthread_mutex_lock(&bt->mutex);
		printf("Recursive - \n");
		inorder_recursive(bt->root);
		printf("\n");
		printf("Non Recursive - \n");
		inorder_non_recursive(bt->root);
		printf("\n");
		pthread_mutex_unlock(&bt->mutex);
	}
}

void binary_tree_postorder_traverse(void *handle)
{
	binary_tree_t *bt = (binary_tree_t *)handle;
	if (NULL != bt && BINARY_TREE_ID == bt->magic)
	{
		pthread_mutex_lock(&bt->mutex);
		printf("Recursive - \n");
		postorder_recursive(bt->root);
		printf("\n");
		printf("Non Recursive - \n");
		postorder_non_recursive(bt->root);
		printf("\n");
		pthread_mutex_unlock(&bt->mutex);
	}
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
	binary_tree_t *bt = (binary_tree_t *)handle;
	status_t ret = BINARY_TREE_INVALID;

	pthread_mutex_lock(&bt->mutex);
	if (NULL != bt && BINARY_TREE_ID == bt->magic)
	{
		node_t *node = calloc(sizeof(node), 1);
		if (node)
		{
			ret = BINARY_TREE_OP_STATUS_PASS;
			node->data = data;
			if (NULL != bt->root)
				binary_search_tree_add(bt->root, node);
			else
				bt->root = node;
			*out = node;
		}
		else
		{
			ret = BINARY_TREE_OP_STATUS_FAIL;
		}
	}
	pthread_mutex_unlock(&bt->mutex);
	return ret;
}
