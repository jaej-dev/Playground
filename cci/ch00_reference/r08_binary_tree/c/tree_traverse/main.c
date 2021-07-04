/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 7. Binary Tree, tree traverse
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

void pre_order(node_t * n)
{
	static int count = 0;
	++count;

	printf("%s: recursive count = %d, node addr = %p, data = %d\n", __func__, count, (void *)n, n->data);

	if (n->left)
		pre_order(n->left);

	if (n->right)
		pre_order(n->right);	

	printf("%s: done. recursive count = %d\n", __func__, count);
}

void in_order(node_t * n)
{
	static int count = 0;
	++count;

	if (n->left)
		in_order(n->left);

	printf("%s: recursive count = %d, node addr = %p, data = %d\n", __func__, count, (void *)n, n->data);

	if (n->right)
		in_order(n->right);
}

void post_order(node_t * n)
{
	static int count = 0;
	++count;

	if (n->left)
		post_order(n->left);

	if (n->right)
		post_order(n->right);	

	printf("%s: recursive count = %d, node addr = %p, data = %d\n", __func__, count, (void *)n, n->data);
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

	init_tree(1);
	left = add_child(gRoot, 2, true);
	right = add_child(gRoot, 3, false);
	add_child(left, 4, true);
	add_child(right, 5, false);

	pre_order(gRoot);
	in_order(gRoot);
	post_order(gRoot);

	deinit_tree(gRoot);

  return 0;
}
