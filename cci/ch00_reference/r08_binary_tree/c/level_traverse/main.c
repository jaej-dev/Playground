/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 7. Binary Tree, level travers with queue
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef struct _node_t {
	int data;
	struct _node_t * left;
	struct _node_t * right;
} node_t;

node_t * gRoot;
node_t ** gQueue;
int gQueueSize;
int gQueueHead;
int gQueueTail;

int init_queue(int size)
{
  gQueueSize = size;
  gQueue = malloc(gQueueSize * sizeof(node_t *));
  assert(gQueue!=NULL);
  gQueueHead = gQueueTail = 0;

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

int insertq(node_t * data)
{
  if ((gQueueTail+1)%gQueueSize == gQueueHead)
  {
    printf("%s: queue is full.\n", __func__);
    return -1;
  }

  gQueue[gQueueTail] = data;
  gQueueTail = (gQueueTail + 1) % gQueueSize;

  printf("%s: slot = %d, data = %p, head = %d, tail = %d\n", __func__, 
    gQueueTail-1, (void *)gQueue[gQueueTail-1], gQueueHead, gQueueTail);

  return 0;
}

node_t * removeq(void)
{
  node_t * data;

  if (gQueueHead == gQueueTail)
  {
    printf("%s: queue is empty.\n", __func__);
    return NULL;
  }

  data = gQueue[gQueueHead];
  gQueueHead = (gQueueHead + 1) % gQueueSize;

  printf("%s: slot = %d, data = %p, head = %d, tail = %d\n", __func__,
    gQueueHead-1, (void *)gQueue[gQueueHead-1], gQueueHead, gQueueTail);

  return data;
}

int print_all_queue(void)
{
  int i;

  printf("%s: ", __func__);

  for(i = gQueueHead; i < gQueueTail; ++i )
    printf("%d-%p\t", i, (void *)gQueue[i]);

  printf("\n");

  return 0;
}


void init_tree(int data)
{
	gRoot = malloc(sizeof(node_t));
	assert(gRoot != NULL);

	gRoot->data = data;
	gRoot->left = NULL;
	gRoot->right = NULL;
	printf("%s: gRoot addr = %p, data = %d\n", __func__, (void *)gRoot, gRoot->data);
}

node_t * add_child(node_t * n, int data, int isLeft)
{
	node_t * new;

	new = malloc(sizeof(node_t));
	new->data = data;
	new->left = NULL;
	new->right = NULL;

	if (isLeft)
		n->left = new;
	else
		n->right = new;

	printf("%s: new = %p, data = %d, post = %s\n", __func__, (void *)new, data, isLeft?"left":"right");
	return new;
}

void level_order(node_t * n)
{
	node_t * temp;

	insertq(n);	// root
	while (gQueueHead != gQueueTail) {
		temp = removeq();
		printf("%s: addr = %p, data = %d\n", __func__, (void *)temp, temp->data);

		if (temp->left)
			insertq(temp->left);
		if (temp->right)
			insertq(temp->right);
	}
}

int deinit_tree(node_t * n)
{
	static int count = 0;
	++count;

	if (n->left)
		deinit_tree(n->left);

	if (n->right)
		deinit_tree(n->right);

	printf("%s: recursive count = %d, addr = %p, data = %d\n", __func__, count, (void *)n, n->data);

	free(n);

	return 0;
}

int main(void)
{
	node_t * left;
	node_t * right;


	init_queue(16);
	init_tree(1);
	left = add_child(gRoot, 2, true);
	right = add_child(gRoot, 3, false);
	add_child(left, 4, true);
	add_child(right, 5, false);

	level_order(gRoot);

	deinit_tree(gRoot);
	deinit_queue();

  return 0;
}
