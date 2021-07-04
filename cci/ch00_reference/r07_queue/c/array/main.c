/*           
 * Title        :   Data Structure Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 6. Array Queue by c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int * gQueue;
int gHead, gTail;
int gQueueSize;


int init_queue(int size)
{
	gQueueSize = size;
	gQueue = malloc(gQueueSize * sizeof(int));
	assert(gQueue!=NULL);
	gHead = gTail = 0;

	printf("%s: queue slot = %d\n", __func__, size);
	return 0;
}

int deinit_queue(void)
{
	if (gQueue)
		free(gQueue);

	printf("%s: done.\n", __func__);

	return 0;
}

int insertq(int data)
{
	if ((gTail+1)%gQueueSize == gHead)
	{
		printf("%s: queue is full.\n", __func__);
		return -1;
	}
	
	gQueue[gTail] = data;
	gTail = (gTail + 1) % gQueueSize;

	printf("%s: slot = %d, data = %d, head = %d, tail = %d\n", __func__, 
		gTail-1, gQueue[gTail-1], gHead, gTail);

	return 0;
}

int removeq(void)
{
	int data;

	if (gHead == gTail)
	{
		printf("%s: queue is empty.\n", __func__);
		return -1;
	}

	data = gQueue[gHead];
	gHead = (gHead + 1) % gQueueSize;

	printf("%s: slot = %d, data = %d, head = %d, tail = %d\n", __func__, 
		gHead-1, gQueue[gHead-1], gHead, gTail);
	return data;
}

int print_all_queue(void)
{
	int i;

	printf("%s: ", __func__);

	for(i = gHead; i < gTail; ++i )
		printf("%d-%d\t", i, gQueue[i]);

	printf("\n");

	return 0;
}

int main(void)
{
	int i;

	init_queue(10);

	for (i = 0; i < gQueueSize-1; ++i)
	{
		insertq(100+i);
	}

	print_all_queue();


	for (i = 0; i < 5; ++i)
		printf("%s: removeq count = %d, data = %d\n", __func__, i, removeq());
	
	print_all_queue();

  return 0;
}
