#include "LinkedList.h"

int main()
{
	void *sll = sll_create();
	void *node[30] = {NULL};
	if (sll)
	{
		int i = 0;
		while (i < 30)
		{
			sll_add(sll, i, &node[i]);
			i++;
		}
		sll_print(sll);
		sll_reverse(sll);
		sll_print(sll);
	}
	return 0;
}
