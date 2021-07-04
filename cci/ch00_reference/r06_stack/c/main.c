/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 5. Stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int * gStackPool;
int gStackSize;
int gStackTop;

int init_stack(int size)
{
	gStackPool = malloc(sizeof(int)*size);
	assert(gStackPool!=NULL);
	gStackSize = size;
	gStackTop = -1;
	printf("%s: created stack pool, size = %d slots\n", __func__, gStackSize);

	return 0;
}

int deinit_stack(void)
{
	free(gStackPool);

	//gStackPool = NULL;
	gStackTop = -1;
	printf("%s: deleted stack pool.\n", __func__);
	return 0;
}

void push(int data)
{
	if (gStackTop < gStackSize-1)
	{
		gStackPool[++gStackTop] = data;
	}
	else
	{
		printf("%s: stack is full.\n", __func__);
		assert(0);
	}
}

int pop()
{
	if (gStackTop > -1)
	{
		return gStackPool[gStackTop--];
	}
	else
	{
		printf("%s: stack is empty.\n", __func__);
		assert(0);
	}

	return -1;
}

int print_all_stack(void)
{
	int i;
	if (gStackTop > gStackSize)
	{
		printf("%s: stack is empty.\b", __func__);
		return -1;
	}

	for(i = 0; i <= gStackTop; ++i)
		printf("%d-%d\t", i, gStackPool[i]);

	printf("\n");
	return 0;
}

int main(void)
{
	int i;
	init_stack(10);

	for (i = 0; i < gStackSize; ++i) /* ori */
//	for (i = 0; i < gStackSize+100; ++i) /* stack overflow */
		push(i);

	print_all_stack();

	for (i = 0; i < gStackTop; ++i)	/* ori */
//	for (i = 0; i < gStackTop+100; ++i) /* stack underflow */
		printf("%s: pop count = %d, data = %d, top = %d\n", __func__, i, pop(), gStackTop);

	print_all_stack();

	deinit_stack();

  return 0;
}
