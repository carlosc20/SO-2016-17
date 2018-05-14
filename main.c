#include "Includes/main.h"
#include <sys/wait.h>


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




void noArgs(int argc){
	if(argc < 2){
		fprintf(stderr, "use: Bash <path>\n");
		exit(1);
	}
}

int openNoteBook(char *path){
	int fd;
	if((fd = open(path, O_RDWR, 0644)) < 0){
		write(2,"Error On Opening File!\n",24);
	}

	return fd;
}


void execCMD(char *str){
	char *cmd=strdup(str);

	switch(cmd[0]){
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
}

void callCMDS(DynaArray *cmds){
	int i = 0;
	while(i < cmds->used){
		if(fork()){
			wait(NULL);
		}
		else{
			char *cmd=strstr(cmds->array[i], "\n");
			cmd += 2;
			execCMD(cmd);
			exit(0);
		}

		i++;
	}
}

int main(int argc, char *argv[]){

	noArgs(argc);

	int fd = openNoteBook(argv[1]);

	//Array Dynamico para guardar as strings (cada string contem a descriçao do comando e o comando)
	DynaArray *cmds = createDynaArray(10);

	//argv[1] é um path para um file para ler!
	separateCMD(cmds, readFile(fd));

	callCMDS(cmds);

	//printDynaArray(cmds);

	close(fd);

	return 0;
}