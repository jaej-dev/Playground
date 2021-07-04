/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 3. Hash by c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUCKET	10
#define SLOT	1

int hash_table[BUCKET][SLOT] = {
	{0},
	{1},
	{2},
	{3},
	{4},
	{5},
	{6},
	{7},
	{8},
	{9},
};

int hash(int key)
{
	return key % 10;
}

int hash2(int key)
{
	return (key / 10 + key % 10) % 10;
}

void add_key(int key)
{
	int bucket;

	bucket = hash(key);
	if (hash_table[bucket][0] != key)
		bucket = hash2(key);
	if (hash_table[bucket][0] == 0)
		hash_table[bucket][0] = key;
}

int find_key(int key)
{
	int bucket;

	bucket = hash(key);
	if (hash_table[bucket][0] == key)
		return 1;
	bucket = hash2(key);
	if (hash_table[bucket][0] == key)
		return 1;

	return 0;
}

void print_table()
{
	int i, j;

	for (i = 0; i < BUCKET; ++i)
		for (j = 0; j < SLOT; ++j)
			printf("buck ket: %d slot: %d value: %d\n", i, j, hash_table[i][j]);
}

int main(void)
{
	int i, key;

	memset(hash_table, 0, sizeof(hash_table));

	for (i = 0; i<5; i++) 
	{
		printf("[%d] press input key: ",i+1);
		scanf("%d",&key);
		add_key(key);
	}

	printf("press search key: ");
	scanf("%d",&key);

	if (find_key(key))
		puts("found key.\n");
	else 
		puts("can not found key.\n");
		
	print_table();

  return 0;
}
