#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DynaArr{
	char **array;
	int used;
	int size;
} DynaArray;

DynaArray *createDynaArray(int size);
void insertDynaArray(DynaArray *a, char *str);
void freeDynaArray(DynaArray *a);
void printDynaArray(DynaArray *a);

#endif