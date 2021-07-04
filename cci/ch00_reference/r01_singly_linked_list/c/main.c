/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 1. Singly Linked List by c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _node_t {
	int data;
	struct _node_t * next;
} node_t;

node_t * gHead;

node_t * init_list(void)
{
	gHead = (node_t *)malloc(sizeof(node_t));
	gHead->data = 0;
	gHead->next = NULL;
	printf("%s: init list, gHead = %p\n",__func__, (void *)gHead);
	return gHead;
}

node_t * ins_node(node_t * t, node_t * n)
{
	node_t * new;
	new = (node_t *)malloc(sizeof(node_t));
	new->data = n->data;
	new->next = NULL;
	t->next = new;
	printf("%s: added addr=%p, data=%d\n", __func__, (void *)new, new->data);
	return new;
}

int del_node(node_t * t)
{
	node_t * del;
	del = t->next;
	if (del == NULL)
	{
		printf("%s: list is empty\n", __func__);
		return -1;
	}

	printf("%s: deleted addr=%p, data=%d\n", __func__, (void *)del, del->data);
	t->next = del->next;
	free(del);
	return 0;	
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
	while(del_node(gHead) > -1);
	free(gHead);
	gHead = NULL;
	printf("%s: deinit list\n", __func__);
	return 0;
}

int main(void)
{
	node_t * now;
	node_t temp;
	int i;

	now = init_list();

	for(i=1; i<6; ++i)
	{
		temp.data = i*100;
		now = ins_node(now, &temp);
	}

	print_all_list();

	deinit_list();

	print_all_list();

  return 0;
}
