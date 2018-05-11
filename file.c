#include "file.h"

#define RDCHAR 1

int num_cmd = 0;

char *readFile(char *path){
	int fd;

	if((fd = open(path, O_RDONLY)) < 0){
		write(2,"Error On Opening File!\n",24);
		exit(2);
	}

	char buff[RDCHAR];
	char *str = malloc(RDCHAR);
	int rd = RDCHAR;

	while(rd == RDCHAR){
		rd = read(fd, buff, RDCHAR);
		
		if(rd != RDCHAR){
			buff[rd] = '\0';
			break;
		}

		strcat(str,buff);
		str = realloc(str, strlen(str)+rd);
	}

	if(close(fd) < 0){
		write(2,"Error On Closing File!\n",24);
		exit(3);
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