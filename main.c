#include "Includes/main.h"
#include "Includes/btree.h"
#include <sys/wait.h>

#include <signal.h>
#include <sys/types.h>

#define BUFFER 512

int FLAG = 0;

//Executa o comando cmd e adiciona o resultado ao ans
void execCMD(DynaArray *ans, char *cmd){
	char *str = strdup(cmd);
	char *string = str;
	int index;

	int p[2];
	pipe(p);
	dup2(p[0], 0);
	close(p[0]);

	while(str[0] == ' '){
		str++;
	}

	if(sscanf(str, "%d|", &index) == 1){
		if(index > ans->length){
			char *error = malloc(31 + strlen(str));
			strcpy(error, "Error on executing command: $");
			strcat(error, str);
			strcat(error, "\n");
			write(2, error, strlen(error));
			free(error);
			exit(5);
		} else {
			str = strstr(str, "|");
			str++;

			write(p[1], ans->array[ans->length - index], strlen(ans->array[ans->length - index]));
		}
	} else if(str[0] == '|'){
		str++;

		write(p[1], ans->array[ans->length - 1], strlen(ans->array[ans->length - 1]));
	}

	close(p[1]);

	pipe(p);
	dup2(p[0], 2);

	execute(str);
	if(read(p[1], NULL, 1) > 0){ //ve se o comando escreveu para o stderr
		exit(1);
	}

	write(1, "", 1);
	close(p[0]);
	close(p[1]);

	free(string);
}

//Executa os comandos do cmds e adicionando os seus resultados ao ans
void callCMDS(DynaArray *cmds, DynaArray *ans){
	for(int i = 0; i < cmds->length; i++){
		int p[2];
		int status;
		pipe(p);

		if(fork()){
			wait(&status);
			if(status != 0){
				exit(2);
			}

			close(p[1]);
			insertDynaArrayNoCpy(ans, readFile(p[0]));
			close(p[0]);
		} else {

			close(p[0]);
			dup2(p[1],1);
			close(p[1]);

			execCMD(ans, cmds->array[i]);
			exit(0);
		}
	}
}


void sighandler(int sig) {
	FLAG = 1;
}


void deleteNotebook(char *path){
	if(unlink(path)){
		write(2, "Error on deleting notebook file.\n", 29);
	}
}

//Cria um notebook novo já processado e apaga o original
void replaceNotebook(char *notebook, char *str){

	char *nb = strdup(notebook);
	char *nb1 = nb;
	//isola nome do ficheiro
	while(strstr(nb, "/")){
		nb = strstr(nb, "/");
		nb++;
	}

	char *path = strndup(notebook, strlen(notebook) - strlen(nb));

	char *sub = malloc(strlen(path) + strlen(nb) + 5);
	strcpy(sub, path);
	strcat(sub, ".TMP");
	strcat(sub, nb);
	free(nb1);
	free(path);

	signal(SIGINT, sighandler); //muda a resposta ao sinal de interrupção para evitar a perda de dados

	//cria novo nb processado -> sub
	int fd = open(sub, O_CREAT | O_WRONLY, 0644);

	if(write(fd, str, strlen(str)) < 0){
		free(str);
		write(2, "Error on writing to TMP file!\n", 31);
		deleteNotebook(sub);
		exit(1);
	}
	//existe novo e original -> em caso de interrupção apaga o novo
	if(FLAG){
		deleteNotebook(sub);
		exit(3);
	}


	//apaga antigo
	if(unlink(notebook)){
		write(2, "Error on deleting notebook file!\n", 34);
		deleteNotebook(sub);
		exit(2);
	}

	//existe só novo
	//neste ponto as interrupções são ignoradas para evitar perda de dados

	//muda nome do ficheiro
	if(link(sub, notebook)){
		write(2, "Error on renaming TMP file!\n", 29);
	}

	//apaga novo
	deleteNotebook(sub);

	signal(SIGINT, SIG_DFL); //reverte as interrupções para o comportamento normal

	free(sub);
}

//junta as Strings numa só
char *combine(DynaArray *descs, DynaArray *cmds, DynaArray *ans){
	int i = 0;
	char *str = malloc(1);

	while(i < cmds->length || i < descs->length || i < ans->length){
		if(i < descs->length){
			str = realloc(str, strlen(str) + strlen(descs->array[i]));
			strcat(str, descs->array[i]);
		}

		if(i < cmds->length){
			str = realloc(str, strlen(str) + strlen(cmds->array[i]) + 1);
			strcat(str, "$");
			strcat(str, cmds->array[i]);
		}

		if(i < ans->length){
			str = realloc(str, strlen(str) + strlen(ans->array[i]) + 9);
			strcat(str, "\n>>>\n");
			strcat(str, ans->array[i]);
			strcat(str, "<<<\n");
		}

		i++;
	}
	return str;
}

int openNotebook(char *path){
	int fd;
	if((fd = open(path, O_RDONLY, 0644)) < 0){
		write(2, "Error on opening notebook!\n", 26);
	}
	return fd;
}


int main(int argc, char *argv[]){
	if(argc < 2){
		write(2, "use: Bash <path>\n", 19);
		exit(1);
	}

	//Array dinamico para guardar os comandos(incluindo a descriçao do mesmo cmd) e os resultados
	DynaArray *cmds = createDynaArray(10);
	DynaArray *descs = createDynaArray(10);
	DynaArray *ans = createDynaArray(10);

	//Le o ficheiro e insere no array cmds os comandos
	int fd = openNotebook(argv[1]);
	separateCMD(cmds, descs, readFile(fd));
	close(fd);


	//Chama um comando de cada vez ordenadamente usando o fork e executa-o, e guarda o resultado no array ans
	callCMDS(cmds, ans);

	//cria um notebook com as respostas que depois substitui o original
	replaceNotebook(argv[1], combine(descs, cmds, ans));

	return 0;
}
