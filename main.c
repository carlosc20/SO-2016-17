#include "Includes/main.h"
#include <sys/wait.h>

#define BUFFER 512

char **cmdArgs(char *cmd){
	char *buff[100];
	char *tok;
	int i = 0;

	tok = strtok(cmd, " ");
	while(tok){
		buff[i] = tok;
		tok = strtok(NULL, " ");
		i++;
	}

	char **args = malloc(i * sizeof(char*));
	int w = 0;
	while(w < i){
		args[w] = buff[w];
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
		write(2, "Error On Opening File!\n", 24);
	}

	return fd;
}


void execCMD(DynaArray *pipes, char *cmd){
	char **args;
	int index;

	int p[2];
	pipe(p);
	dup2(p[0], 0);
	close(p[0]);

	switch(cmd[0]){
		case ' ':
			args = cmdArgs(cmd+1);
			break;
		case '|':
			args = cmdArgs(cmd+2);
			//Verificar erros!!!!
			write(p[1], pipes->array[pipes->length - 1], strlen(pipes->array[pipes->length - 1])); //Pode ser melhorado
			break;
		default:
			if(sscanf(cmd, "%d| %s", &index, cmd) == 2){
				args = cmdArgs(cmd);
				//Verificar erros!!!!
				write(p[1], pipes->array[pipes->length - index], strlen(pipes->array[pipes->length - index])); //Pode ser melhorado
			}
			break;
	}
	close(p[1]);

	execvp(args[0], args);
	exit(1); // Não foi possivel executar o comando, dar KILL;
}

void callCMDS(DynaArray *cmds, DynaArray *ans){
	int i = 0;
	while(i < cmds->length){
		int p[2];
		pipe(p);

		if(fork()){
			close(p[1]);
			insertDynaArrayNoCpy(ans, readFile(p[0]));
			close(p[0]);
		} else {

			close(p[0]);
			dup2(p[1],1);
			close(p[1]);

			execCMD(ans, cmds->array[i]);

			exit(1); //Não foi possivel executar o comando
		}

		i++;
	}
}

int main(int argc, char *argv[]){

	noArgs(argc);

	int fd = openNoteBook(argv[1]);

	//Array Dynamico para guardar os comandos(incluindo a descriçao do mesmo cmd) e os resultados
	DynaArray *cmds = createDynaArray(10);
	DynaArray *descs = createDynaArray(10);
	DynaArray *ans = createDynaArray(10);
	
	//Le o ficheiro (notebook.txt) e insere no array cmds os comandos
	separateCMD(cmds, descs, readFile(fd));

	//Chamada um comando de cada vez usando o fork e executa-o, escrevendo o resultado num array dinamico (ans)
	callCMDS(cmds, ans);

	printDynaArray(ans);

	close(fd);

	return 0;
}
