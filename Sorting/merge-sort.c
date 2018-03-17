#include "stdio.h"
#define swap(a, b) do { \
int temp = (a); \
(a) = (b); \
(b) = temp; \
} while (0);

/* Merge sort without using extra memory space during merging */
void merge_sort(int arr[], int a, int b);

int merge(int arr[], int a, int b, int i, int j)
{
	if (a == b && (i == j || arr[a] <= arr[i]))
	{
		if (arr[a] > arr[i])
			swap(arr[a], arr[i]);
		arr[i - 1] = arr[a];
		return i - 2;
	}
	else
	{
		int k = 0;
		int temp = 0;
		if (arr[a] <= arr[i])
		{
			temp = arr[a];
			k = merge(arr, a + 1, b, i, j);
			arr[k] = temp;
			return k - 1;
		}
		else
		{
			temp = arr[i];
			k = merge(arr, a, b, i + 1, j);
			arr[k] = temp;
			return k - 1;
		}
	}
}

void merge_sort(int arr[], int a, int b)
{
	if (a >= b)
	{
		return;
	}
	else if (b == a + 1)
	{
		if (arr[a] > arr[b])
			swap(arr[a], arr[b]);
		return;
	}
	else
	{	
		divide(arr, a, (a + b) / 2);
		divide(arr, ((a + b) / 2) + 1, b);
		merge(arr, a, (a + b) / 2, (a + b) / 2 + 1, b);
	}
}


