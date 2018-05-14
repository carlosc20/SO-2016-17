#include "Includes/struct.h"

DynaArray *createDynaArray(int size){
	DynaArray *a;

	a = malloc(sizeof(DynaArray));
	a -> array = malloc(size * sizeof(char *));
	a -> used = 0;
	a -> size = size;

	return a;
}

void insertDynaArray(DynaArray *a, char *str){
	if (a->used == a->size){
		a->size++;
    	a->array = (char **)realloc(a->array, a->size * sizeof(char*));
	}
	a->array[a->used++] = strdup(str);
}

void freeDynaArray(DynaArray *a){
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

void printDynaArray(DynaArray *a){
	for(int i=0; i<a->used; i++){
		printf("%s\n", a->array[i]);
	}
}