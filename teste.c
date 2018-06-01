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

	while(str[0] != '\0'){
		if(str[0] == ' '){
			str++;
			aux=strndup(buff, i);
			i=0;
			printf("%s\n", aux);
		}
		else{
			buff[i]=str[0];
			i++;
			str++;
		}
	}

	return 0;
}