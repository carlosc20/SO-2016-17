#include "Includes/main.h"
#include <sys/wait.h>

#define BUFFER 512

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

char *readPipe(int p){
	char buff[BUFFER];
	char *str;
	int len = 0;
	int rd;

	while((rd = read(p, buff, BUFFER)) > 0){
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

void callCMDS(DynaArray *cmds, DynaArray *ans){
	int i = 0;
	while(i < cmds->used){
		int p[2];
		pipe(p);

		if(fork()){
			close(p[1]);

			insertDynaArray(ans, readPipe(p[0]));

			close(p[0]);
		}
		else{
			char *cmd=strstr(cmds->array[i], "\n");
			cmd += 2;

			close(p[0]);
			dup2(p[1],1);
			close(p[1]);

			execCMD(cmd);

			exit(0);
		}

		i++;
	}
}

int main(int argc, char *argv[]){

	noArgs(argc);

	int fd = openNoteBook(argv[1]);

	//Array Dynamico para guardar os comandos(incluindo a descriçao do mesmo cmd) e os resultados
	DynaArray *cmds = createDynaArray(10);
	DynaArray *ans = createDynaArray(10);

	//Le o ficheiro (notebook.txt) e insere no array cmds os comandos
	separateCMD(cmds, readFile(fd));

	//Chamada um comando de cada vez usando o fork e executa-o, escrevendo o resultado num array dinamico (ans)
	callCMDS(cmds, ans);

	printDynaArray(ans);

	close(fd);

	return 0;
}
