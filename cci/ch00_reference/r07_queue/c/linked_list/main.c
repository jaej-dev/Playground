/*           
 * Title        :   Data Structure Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 6. Linked List Queue by c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef struct _node_t {
	int data;
	struct _node_t * prev;
	struct _node_t * next;
} node_t;

node_t * gHead;

void init_queue(void)
{
	gHead = malloc(sizeof(node_t));
	assert(gHead != NULL);

	gHead->prev = NULL;
	gHead->next = NULL;
	printf("%s: gHead = %p\n", __func__, (void *)gHead);
}

int insertq(int data)
{
	node_t * new;
	node_t * tail;

	for(tail = gHead; tail->next; tail = tail->next);

	new = malloc(sizeof(node_t));
	new->data = data;
	new->next = NULL;
	new->prev = tail;
	tail->next = new;

	printf("%s: new = %p, data = %d, head = %p, tail = %p\n", __func__, (void *)new, 
		data, (void *)gHead, (void *)tail);
	return 0;
}

int removeq(void)
{
	int data;
	node_t * target;

	target = gHead->next;
	if (target == NULL)
	{
		printf("%s: queue is empty\n", __func__);
		return -1;
	}

	data = target->data;
	gHead->next = target->next;

	if (gHead->next)
		gHead->next->prev = gHead;

	free(target);

	printf("%s: data = %d, head = %p\n", __func__, data, (void *)gHead);
	return data;
}

int deinit_queue(void)
{
	printf("%s: start\n", __func__);

	while(removeq() != -1);
	free(gHead);

	printf("%s: done\n", __func__);

	return (int)0;
}

int print_all_queue(void)
{
	node_t * temp;
	int i = 0;

	temp = gHead->next;

	if (temp == NULL)
	{
		printf("%s: empty\n", __func__);
		return 0;
	}

	printf("%s:\n", __func__);

	for (i = 0; temp; temp = temp->next, ++i)
	{
		printf("slot %d-%d\t", i, temp->data);
	}

	printf("\n");

	return 0;
}

int main(void)
{
	int i;

	init_queue();

	for (i = 0; i < 10; ++i)
	{
		insertq(100+i);
	}

	print_all_queue();

	for (i = 0; i < 5; ++i)
	{
		printf("%s: remove count = %d, queue data = %d\n", __func__, i, removeq());
	}

	print_all_queue();

	deinit_queue();

  return 0;
}