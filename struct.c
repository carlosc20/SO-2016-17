#include "Includes/struct.h"

DynaArray *createDynaArray(int size){
	DynaArray *a;

	a = malloc(sizeof(DynaArray));
	a->array = malloc(size * sizeof(char *));
	a->length = 0;
	a->size = size;

	return a;
}

void insertDynaArray(DynaArray *a, char *str){
	if (a->length == a->size){
		a->size *= 2;
    	a->array = (char **) realloc(a->array, a->size * sizeof(char*));
	}
	a->array[a->length++] = strdup(str);
}

void insertDynaArrayNoCpy(DynaArray *a, char *str){
	if (a->length == a->size){
		a->size *= 2;
    	a->array = (char **) realloc(a->array, a->size * sizeof(char*));
	}
	a->array[a->length++] = str;
}

void freeDynaArray(DynaArray *a){
  free(a->array);
  a->array = NULL;
  a->length = a->length = 0;
}

void printDynaArray(DynaArray *a){
	for(int i=0; i < a->length; i++){
		printf("%s\n", a->array[i]);
	}
}
