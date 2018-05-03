#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

char *readFile(char *path){
	int fd = open(path, O_RDONLY);
	char *aux=malloc(100);
	char *str=malloc(100);
	int rd=100;

	while(rd==100){
		rd=read(fd, aux, 100);
		strcat(str,aux);
		if(rd!=100){
			str[strlen(str)-100+rd]='\0';
			break;
		}
		str=realloc(str, strlen(str)+100);
	}

	if(close(fd)<0){printf("Error!!\n");}
	
	printf("%s\n", str);

	return str;
}

int main(int argc, char *argv[]){
	//argv[1] é um path para um file para ler!
	readFile(argv[1]);
	return 0;
}