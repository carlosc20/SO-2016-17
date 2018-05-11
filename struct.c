#include "struct.h"

DynaArray *createDynaArray(int size){
	DynaArray *a;
	a -> array = malloc(size * sizeof(char *));
	a -> used = 0;
	a -> size = size;

	return a;
}

void insertDynaArray(DynaArray *a, char *str){
	if (a->used == a->size){
		a->size++;
    	a->array = (char **)realloc(a->array, a->size * sizeof(char *));
	}

	a->array[a->used++] = str;
}

void freeDynaArray(DynaArray *a){
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}