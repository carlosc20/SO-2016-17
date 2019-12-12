#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DynaArr{
	char **array;
	int length;
	int size;
} DynaArray;

DynaArray *createDynaArray(int size);
void insertDynaArrayNoCpy(DynaArray *a, char *str);
void freeDynaArray(DynaArray *a);
void printDynaArray(DynaArray *a);

#endif
