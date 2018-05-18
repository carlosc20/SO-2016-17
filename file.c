#include "Includes/file.h"

#define BUFFER 512

char *readFile(int fd){
	char buff[BUFFER];
	char *str;
	int len = 0;
	int rd;

	while((rd = read(fd, buff, BUFFER)) > 0){
		if(len){
			len += rd;
			str = realloc(str, len);
			strncat(str, buff, rd);
		} else {
			len = rd;
			str = strndup(buff, rd);
		}
	}
	if(rd < 0){
		//Algo correu mal
	}
	return str;
}


//Separa a String (c/ os CMD) e guarda-os num array de strings
void separateCMD(DynaArray *cmds, DynaArray *descs, char *cmd){
	int flag = 1;
	char *start = cmd;
	char *end;
	char *aux;
	char *desc = cmd;
	char buff[BUFFER];

	while(start && (start = strstr(start, "$"))){
		aux = strndup(desc, start - desc); //Copia a descrição
		insertDynaArray(descs, aux);

		end = strstr(start, "\n"); //Procura o fim da linha
		desc = end + 1;

		if(end) {
			aux = strndup(start + 1, end - start - 1);
		} else {
			aux = strdup(start + 1);
		}

		insertDynaArray(cmds, aux);
		start = end;
	}
	aux = strdup(desc);
	insertDynaArray(descs, aux);
}
