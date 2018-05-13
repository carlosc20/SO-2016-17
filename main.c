#include "main.h"

void print_CMD(char *cmds[]){
	int i = 0;
	while(i < 3){
		printf("%s\n", cmds[i]);
		i++;
	}
}

char **cmdArgs(char *cmd){
	char *buff[100];
	char *tok;
	int i=0;

	tok=strtok(cmd," ");
	while(tok){
		buff[i]=tok;
		tok=strtok(NULL," ");
		i++;
	}

	char **args=malloc(i*sizeof(char*));
	int w=0;
	while(w<i){
		args[w]=buff[w];
		w++;
	}

	return args;
}

void execCMD(DynaArray *cmds, int fd){
	int i = 0;

	while(i < cmds->used){
		char *cmd = strstr(cmds->array[i],"\n");
		cmd+=2;
		switch(cmd[i]){
			case ' ':
				cmd++;
				char **args=cmdArgs(cmd);
				execvp(args[0], args);
				break;
			case '|':
				break;
			default:
				break;
		}
		i++;
	}
}

int main(int argc, char *argv[]){

	if(argc < 2){
		fprintf(stderr, "use: Bash <path>\n");
		exit(1);
	}

	int res = open("respostas.txt", O_RDWR | O_CREAT, 0644);

	//Array Dynamico para guardar as strings (cada string contem a descriçao do comando e o comando)
	DynaArray *cmds = createDynaArray(10);
	
	//argv[1] é um path para um file para ler!
	separateCMD(cmds, readFile(argv[1]));

	//printDynaArray(cmds);

	//Executa os comandos
	execCMD(cmds, res);

	return 0;
}