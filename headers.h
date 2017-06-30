#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<error.h>
#include<errno.h>
#include<x86_64-linux-gnu/sys/cdefs.h>

#define uch unsigned char
#define ui unsigned int
#define us unsigned short

#define displayMatrix(matrix) { int i,j;\
	for(i=0;i<16;i++){\
		for(j=0;j<15;j++){\
			printf("%d ",matrix[i][j]);\
		}\
		printf("%d\n",matrix[i][15]);\
	}\
}

#ifndef NQ4
#define NQ4 ((1<<28))
#endif

