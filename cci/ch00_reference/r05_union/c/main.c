/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 4. Union
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef union _reg_t {
	unsigned int U;
	struct _hw_reg_bitfield_t {
		unsigned int a : 4;	// bit 0:3
		unsigned int b : 4;	// bit 4:7
		unsigned int c : 4; // bit 8:11
		unsigned int d : 4; // bit 12:15
		unsigned int e : 4; // bit 16:19
		unsigned int f : 4; // bit 20:23
		unsigned int g : 4; // bit 24:27
		unsigned int h : 4; // bit 28:31
	} B;
} reg_t;

reg_t * gReg;

int init(void)
{
	gReg = malloc(sizeof(reg_t));
	if (gReg)
		printf("reg base addr = %p, size = %d\n", (void *)gReg, (int)sizeof(reg_t));

	return 0;
}

int fill_reg_value(void)
{
	gReg->U = 0x12345678;
	return 0;
}

void print_reg()
{
	printf("reg addr = %p, data.U = 0x%x\n", (void *)gReg, gReg->U);
	printf("U.a = 0x%x\n", gReg->B.a);
	printf("U.b = 0x%x\n", gReg->B.b);
	printf("U.c = 0x%x\n", gReg->B.c);
	printf("U.d = 0x%x\n", gReg->B.d);
	printf("U.e = 0x%x\n", gReg->B.e);
	printf("U.f = 0x%x\n", gReg->B.f);
	printf("U.g = 0x%x\n", gReg->B.g);
	printf("U.h = 0x%x\n", gReg->B.h);
}

int deinit()
{
	if (gReg)
		free(gReg);

	return 0;
}

int main(void)
{
	init();
	fill_reg_value();
	print_reg();
	deinit();
  return 0;
}
