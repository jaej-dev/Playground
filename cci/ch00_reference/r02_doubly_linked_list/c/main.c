/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 2. Doubly Linked List by c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _node_t {
	int data;
	struct _node_t * prev;
	struct _node_t * next;
} node_t;

node_t * gHead;

static int gHeapCount = 0;

node_t * init_list(void)
{
	gHeapCount = 0;
	gHead = (node_t *)malloc(sizeof(node_t));
	gHead->data = 0;
	gHead->prev = NULL;
	gHead->next = NULL;
	printf("%s: init list\n",__func__);
	return gHead;
}

node_t * ins_node_right(node_t * t, node_t * n)
{
	node_t * new;
	node_t * right;

	new = (node_t *)malloc(sizeof(node_t));
	*new = *n;
	right = t->next;

	t->next = new;
	new->prev = t;
	if (right!=NULL)
		new->next = right;
		
	++gHeapCount;
	printf("%s: added addr=%p, data=%d, heap count = %d\n", __func__, 
		(void *)new, new->data, gHeapCount);
	return new;
}

node_t * ins_node_left(node_t * t, node_t * n)
{
	node_t * new;
	node_t * left;

	new = (node_t *)malloc(sizeof(node_t));
	*new = *n;
	left = t->prev;

	t->prev = new;
	new->next = t;
	if (left!=NULL)
		left->next = new;	
		
	++gHeapCount;
	printf("%s: added addr=%p, data=%d, heap count = %d\n", __func__, 
		(void *)new, new->data, gHeapCount);
	return new;
}

node_t * ins_node_append(node_t * n)
{
	node_t * new;
	node_t * tail;
	
	new = malloc(sizeof(node_t));
	*new = *n;	
	tail = gHead;
	while(tail->next != NULL)
		tail = tail->next;

	tail->next = new;
	new->prev = tail;
	new->next = NULL;

	++gHeapCount;
	printf("%s: tail addr=%p, data=%d\n", __func__, (void *)tail, tail->data);
	printf("%s: added addr=%p, data=%d\n", __func__, (void *)new, new->data);
	printf("%s: heap count %d\n", __func__, gHeapCount);

	return new;
}

int del_node(node_t * t)
{
	node_t * prev;
	node_t * next;

	if (t==NULL || t==gHead)
	{
		printf("%s: not avariable to delete, target is the head or null.\n",__func__);
		return -1;
	}

	prev = t->prev;
	next = t->next;
	prev->next = next;
	if (next != NULL)
		next->prev = prev;

	--gHeapCount;
	printf("%s: deleted addr=%p, data=%d, heap count = %d\n", __func__, 
		(void *)t, t->data, gHeapCount);
	free(t);
	t = NULL;
	return 0;	
}

int get_list_count()
{
	node_t * now;
	int count = 0;

	if(gHead == NULL)
		return 0;

	for (now = gHead; now->next != NULL; now = now->next)
		++count;

	return count;
}

int print_all_list()
{
	node_t * temp = gHead;
	int i = 0;

	while(temp != NULL)
	{
		printf("%s: list %d = %d\n", __func__, i++, temp->data);
		temp = temp->next;
	}
	return 0;
}

int deinit_list(void)
{
	while(del_node(gHead->next) > -1);
	free(gHead);
	gHead = NULL;
	printf("%s: deinit list\n", __func__);
	return 0;
}

int main(void)
{
	node_t temp;
	node_t * now;
	int i;

	init_list();

	for(i=1; i<6; ++i)
	{
		temp.data = i*100;
		now = ins_node_append(&temp);
	}

	printf("%s: list count = %d\n", __func__,  get_list_count());
	print_all_list();

	temp.data = 888;
	now = ins_node_left(now, &temp);

	printf("%s: list count = %d\n", __func__,  get_list_count());
	print_all_list();

	temp.data = 999;
	now = ins_node_right(now, &temp);

	printf("%s: list count = %d\n", __func__,  get_list_count());
	print_all_list();

	deinit_list();
	printf("%s: list count = %d\n", __func__,  get_list_count());
	get_list_count();

  return 0;
}
