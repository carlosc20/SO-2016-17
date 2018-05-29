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

int main(int argc, char const *argv[]){
	cmdArgs("| wc -l -w -a")
	return 0;
}