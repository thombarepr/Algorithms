#include "stdio.h"
#include "stdlib.h"

typedef enum
{
	SLL_OP_STATUS_PASS = 0,
	SLL_OP_STATUS_FAIL,
	SLL_INVALID,
} status_t;

void *sll_create();	
status_t sll_add(void *, int, void**);
status_t sll_delete(void *, void *);
status_t sll_print(void *);
status_t sll_reverse(void *);
