/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 0. Function pointer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int (*FUNCPTR)(void);
typedef int JINT32;

typedef struct _drv_context_t {
	unsigned int id;
	char * name;
} drv_context_t;

typedef struct _drv_func_entry_t {
	void (*init) (drv_context_t *);
	void (*deinit) (drv_context_t *);
} drv_func_entry_t;


#define LOCAL static
#define MERGE(a,b)	b##a
#define MERGE2(a,b) b##b##a##a

static char usage_print[] = "hello function pointer\n";

LOCAL void sample_a_drv_init(drv_context_t * pDrv)
{
	printf("%s: drv = %p, index = %d, name = %s\n", __func__, (void *)pDrv, pDrv->id, pDrv->name);
}

void sample_a_drv_deinit(drv_context_t * pDrv)
{
	printf("%s: drv = %p, index = %d, name = %s\n", __func__, (void *)pDrv, pDrv->id, pDrv->name);
}


void sample_b_drv_init(drv_context_t * pDrv)
{
	printf("%s: drv = %p, index = %d, name = %s\n", __func__, (void *)pDrv, pDrv->id, pDrv->name);
}

void sample_b_drv_deinit(drv_context_t * pDrv)
{
	printf("%s: drv = %p, index = %d, name = %s\n", __func__, (void *)pDrv, pDrv->id, pDrv->name);
}


int main(void)
{
	drv_context_t * drv_a;
	drv_func_entry_t * drv_entry_a;

  printf("%s", usage_print);

	drv_a = malloc(sizeof(drv_context_t));
	assert(drv_a != NULL);
	drv_a->id = 1;
	drv_a->name = "sample a";

	drv_entry_a = malloc(sizeof(drv_func_entry_t));
	assert(drv_entry_a != NULL);


	drv_entry_a->init = sample_a_drv_init;
	drv_entry_a->deinit = sample_a_drv_deinit;

	(*drv_entry_a->init)(drv_a);
	drv_entry_a->deinit(drv_a);

	free(drv_a);	
	free(drv_entry_a);

	printf("a = %d, b = %d, merge1 = %d, merge2 = %d\n", 1, 2, MERGE(1,2), MERGE2(1,2));

  return 0;
}
