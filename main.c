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
		printf("Error On Closing File!\n");
		exit(1);
	}

	return str;
}


//Separa a String (c/ os CMD) e guarda-os num array de strings
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
		num_cmd++;
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

void execCMD(char *cmds[], int fd){
	int i=0;
	
	dup2(fd,1);
	close(fd);

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

	int res = open("respostas.txt", O_RDWR | O_CREAT, 0644);

	//Array para guardar as strings (cada string contem a descriçao do comando e o comando)
	char *cmds[num_cmd];

	//argv[1] é um path para um file para ler!
	separateCMD(readFile(argv[1]), cmds);

	//Print o array com os comandos
	//print_CMD(cmds);

	//Executa os comandos
	execCMD(cmds, res);

	return 0;
}