#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define RDCHAR 1

int num_cmd = 0;

char *readFile(char *path){
	int fd;

	if((fd = open(path, O_RDONLY)) < 0){
		printf("Error On Opening File!\n");
		exit(0);
	}

	char buff[RDCHAR];
	char *str = malloc(0);
	int rd = RDCHAR;

	while(rd == RDCHAR){
		rd = read(fd, buff, RDCHAR);
		str = realloc(str, strlen(str)+rd);
		
		if(rd != RDCHAR){
			buff[rd] = '\0';
		}

		strcat(str,buff);
	}

	if(close(fd) < 0){
		printf("Error On Closing File!\n");
		exit(1);
	}

	return str;
}

char *separateCMD(char *cmd, char *cmds[]){
	int c = 0, i = 0, w = 0;
	char buff[250];
	
	char *str = malloc(strlen(cmd)+1);
	strcpy(str,cmd);

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

		char *aux=malloc(strlen(buff)+1);
		strcpy(aux,buff);

		cmds[w]=aux;
		w++;
	}
	
	return str;
}

void print_CMD(char *cmds[]){
	int i = 0;
	while(i < num_cmd){
		printf("%s\n", cmds[i]);
		i++;
	}
}

void countCMD(char *cmd){
	char *str=malloc(strlen(cmd)+1);
	char *ptr=str;
	strcpy(str,cmd);

	while(str[0]!='\0'){
		if(str[0]=='$'){num_cmd++;}
		str++;
	}

	free(ptr);
}

void execCMD(char *cmds[]){
	int i=0;
	
	while(i<num_cmd){
		char *cmd=malloc(strlen(strstr(cmds[i],"\n")));
		cmd=strstr(cmds[i],"\n");
		cmd+=2;
		switch(cmd[i]){
			case ' ':
				cmd++;
				execlp(cmd, cmd, NULL);
				break;
			case '|':
				break;
			default:
				break;
		}
	}
}

int main(int argc, char *argv[]){

	if(argc < 2){
		fprintf(stderr, "use: Bash <path>\n");
		exit(0);
	}

	//Contas os comandos e guarda numa var global
	countCMD(readFile(argv[1]));


	//Array para guardar as strings (cada string contem a descriçao do comando e o comando)
	char *cmds[num_cmd];

	//argv[1] é um path para um file para ler!
	separateCMD(readFile(argv[1]), cmds);

	//Print o array com os comandos
	//print_CMD(cmds);

	execCMD(cmds);

	return 0;
}