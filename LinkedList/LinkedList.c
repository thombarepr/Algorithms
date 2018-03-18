#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "LinkedList.h"

typedef struct node
{
	int data;
	struct node *next;
} node_t;

typedef struct single_linked_list
{
	int magic;
	node_t *root;
	node_t *end;
	pthread_mutex_t mutex;
} sll_t;

/* Macros for CHECK on linked list handler recived from user
*  Args - 1. Handler recived from user
*         2. Name for sll_t pointer, the user pointer will be typecasted to
*	  3. status_t return variable name. User can chose to modify it between BEGIN
*	     and END and use it as return value.
*  Precaution - This macro lock the mutex, so following must be avoided
*  1. Function return allowed between BT_CHECK_BEGIN and BT_CHECK_END 
*  2. Attempt to lock mutex between BT_CHECK_BEGIN and BT_CHECK_END
*/
#define SLL_CHECK_BEGIN(handle, sll, ret);	\
	sll_t *sll = (sll_t *)handle;	\
	status_t ret = SLL_INVALID;	\
	if (NULL != sll && SINGLE_LINKED_LIST_ID == sll->magic)	\
	{	\
		pthread_mutex_lock(&sll->mutex);	\
		ret = SLL_OP_STATUS_PASS;

#define SLL_CHECK_END(sll, ret);	\
		pthread_mutex_unlock(&sll->mutex);	\
	}	

#define SINGLE_LINKED_LIST_ID 	(0x5A6C)	

status_t sll_print(void *handle)
{
	SLL_CHECK_BEGIN(handle, sll, ret);
	node_t *tmp = sll->root;
	while (tmp)
	{
		printf("%d\t", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");
	ret = SLL_OP_STATUS_PASS;
	SLL_CHECK_END(sll, ret);
	return ret;
}

status_t sll_add(void *handle, int data, void **out)
{
	SLL_CHECK_BEGIN(handle, sll, ret);
	node_t *node = calloc(sizeof(node), 1);
	if (node)
	{
		ret = SLL_OP_STATUS_PASS;
		node->data = data;
		if (NULL != sll->root)
			sll->end->next = node;
		else
			sll->root = node;
		sll->end = node;
		*out = node;
	}
	else
	{
		ret = SLL_OP_STATUS_FAIL;
	}
	SLL_CHECK_END(sll, ret);
	return ret;
}

node_t *sll_reverse_non_recursive(node_t *root)
{
	node_t *tmp = NULL;
	node_t *new_root = NULL;

	while (root)
	{
		tmp = root;
		root = root->next;
		tmp->next = new_root;
		new_root = tmp;
	}
	return new_root;
}

node_t *sll_recurse(node_t *root)
{
	node_t *tmp = NULL;
	if (root)
	{
		tmp = sll_recurse(root->next);
		if (tmp)
		{
			if (tmp->next)
				root->next = tmp->next;
			else
				root->next = tmp;
			tmp->next = root;
		}
	}
	return root;
}

node_t *sll_reverse_recursive(node_t *root)
{
	node_t *last_node = sll_recurse(root);
	node_t *new_root = last_node->next;
	last_node->next = NULL;
	return new_root;
}

status_t sll_reverse(void *handle)
{
	SLL_CHECK_BEGIN(handle, sll, ret);
	sll->root = sll_reverse_recursive(sll->root);
	SLL_CHECK_END(sll, ret);
	return ret;
}

void *sll_create(void)
{
	sll_t *sll = calloc(sizeof(sll_t), 1);

	if (sll)
	{
		sll->magic = SINGLE_LINKED_LIST_ID;
		pthread_mutex_init(&sll->mutex, NULL);
	}
	return sll;
}

