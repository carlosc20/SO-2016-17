#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	char **args = malloc(1);
	char buff[100];
	char *aux;
	char *str = strdup("echo \"Ola tudo bem?\"");
	int i=0;

	while(str[0] == ' '){
		str++;
	}

	while(str){
		if(str[0] == ' '){
			aux=strdup(buff);
			str++;
			printf("%s\n", aux);
			//args = realloc(args, sizeof());
			continue;
		}

		buff[i]=str[0];
		str++;
	}

	return 0;
}