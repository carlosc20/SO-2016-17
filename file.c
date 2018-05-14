#include "Includes/file.h"

#define BUFFER 512

char *readFile(int fd){
	char buff[BUFFER];
	char *str = malloc(BUFFER);
	int len = 0;
	int rd;
	while((rd = read(fd, buff, BUFFER)) > 0){
		len += rd;
		if(len){
			str = realloc(str, len);
			strcat(str, buff);
		} else {
			str = malloc(rd);
		}
	}
	if(rd < 0){
		//Algo correu mal
	}

	return str;
}


//Separa a String (c/ os CMD) e guarda-os num array de strings
char *separateCMD(DynaArray *cmds, char *cmd){
	int c = 0, i = 0;

	char buff[BUFFER];
	char *str = strdup(cmd); //desnecessário duplicar o cmd

	while(str[0] != '\0'){
		while(c < 2 && str[0] != '\0'){
			if(str[1] == '\n'){c++;}
			buff[i] = str[0];
			str++; // Estás a perder o apontador para o inicio do malloc (não dá para depois dar free)
			i++; // Em vez de str[0] -> str[i] ?
		}

		str++; // Estás a perder o apontador para o inicio do malloc (não dá para depois dar free)
		buff[i]='\0';

		i = 0;
		c = 0;

		char *aux = strdup(buff);

		insertDynaArray(cmds, aux);
	}

	return str;
}
