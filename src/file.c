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
		fprintf(stderr, "Error on reading file\n");
	}
	return str;
}


//Recebe uma string e ignora o incio >>> até ao <<<
char* filterAnswers(char* str){
	char* aux;
	char* desc = str;
	if(strncmp(desc, ">>>\n", 4) == 0){
		aux = desc;
		if((desc = strstr(desc, "<<<\n")) == 0){
			desc = aux;
		} else {
			desc += 4;
		}
	}
	return desc;
}

//Separa a String dos comandos e documentação e guarda nos arrays
void separateCMD(DynaArray *cmds, DynaArray *descs, char *cmd){
    char *start = cmd;
    char *end;
    char *aux;
    char *desc = cmd;

    while(start && (start = strstr(start, "$"))){
		desc = filterAnswers(desc); // Retorna o inicio da descrição sem as respostas
        aux = strndup(desc, start - desc); // Duplica a descrição
        insertDynaArrayNoCpy(descs, aux); // Insere a descrição no array das descrições sem copiar
        end = strstr(start, "\n"); // Procura o fim da linha

        if(end) { 
            aux = strndup(start + 1, end - start - 1); // Duplica o comando sem o $ e o \n
            desc = end + 1; // Define o início da descrição para o caracter depois do \n
        } else {
            aux = strdup(start + 1); // Duplica o comando sem o $ até ao fim do ficheiro
            desc = ""; 
        }

        insertDynaArrayNoCpy(cmds, aux); // Insere o comando no array dos comandos
        start = end; // Continua a procurar na próxima linha
	}
	desc = filterAnswers(desc); 
    aux = strdup(desc); 
    insertDynaArrayNoCpy(descs, aux); 
}
