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
void separateCMD(DynaArray *cmds, char *cmd){
	int flag = 1;
	char *begin = cmd;
	char *end;
	char *aux;
	int len;
	char buff[BUFFER];

	while(begin){
		begin = strstr(begin, "$") + 1; //Procura o incio do comando
		end = strstr(begin, "\n"); //Procura o fim da linha

		if(end){
			len = end - begin; //Tamanho da string
			aux = strndup(begin, len);
			begin = end + 1; //Avança para o próximo
		} else {
			aux = strdup(begin);
			begin = NULL; //Para o ciclo
		}

		insertDynaArray(cmds, aux);
	}
}
