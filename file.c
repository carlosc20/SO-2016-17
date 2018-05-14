#include "Includes/file.h"

#define RDCHAR 1

int num_cmd = 0;

char *readFile(int fd){
	char buff[RDCHAR];
	char *str = malloc(RDCHAR);
	int rd = RDCHAR;

	while(rd == RDCHAR){
		rd = read(fd, buff, RDCHAR);
		
		if(rd != RDCHAR){
			buff[rd] = '\0';
			strcat(str,buff);
			break;
		}

		strcat(str,buff);
		str = realloc(str, strlen(str)+rd);
	}

	return str;
}


//Separa a String (c/ os CMD) e guarda-os num array de strings
char *separateCMD(DynaArray *cmds, char *cmd){
	int c = 0, i = 0;
	
	char buff[250];	
	char *str = strdup(cmd);

	while(str[0] != '\0'){
		while(c < 2 && str[0] != '\0'){
			if(str[1] == '\n'){c++;}
			buff[i] = str[0];
			str++;
			i++;
		}

		str++;
		buff[i]='\0';

		i = 0;
		c = 0;

		char *aux = strdup(buff);

		insertDynaArray(cmds, aux);

		num_cmd++;
	}
	
	return str;
}