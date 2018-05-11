#include "main.h"

void print_CMD(char *cmds[]){
	int i = 0;
	while(i < 3){
		printf("%s\n", cmds[i]);
		i++;
	}
}

void execCMD(char *cmds[], int fd){
	int i = 0;
	
	dup2(fd,1);
	close(fd);

	while(i < 3){
		char *cmd = malloc(strlen(cmd = strstr(cmds[i],"\n")));
		cmd += 2;
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
		exit(1);
	}

	int res = open("respostas.txt", O_RDWR | O_CREAT, 0644);

	//Array Dynamico para guardar as strings (cada string contem a descriçao do comando e o comando)
	DynaArray *cmds = createDynaArray(10);

	//argv[1] é um path para um file para ler!
	separateCMD(cmds, readFile(argv[1]));

	//Executa os comandos
	//execCMD(cmds, res);

	return 0;
}