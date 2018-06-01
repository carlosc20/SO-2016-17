#include "Includes/btree.h"

typedef struct BinaryTree{
    char* value;
    struct BinaryTree* left;
    struct BinaryTree* right;
}* BTree;

static BTree newNode(const char* value, BTree left, BTree right){
    BTree newTree = malloc(sizeof(struct BinaryTree));
    newTree->value = strdup(value);
    newTree->left = left;
    newTree->right = right;
    return newTree;
}

static int isProgram(BTree tree){
    return tree->left == NULL && tree->right == NULL;
}

static int isOperator(BTree tree){
    return tree->left != NULL || tree->right != NULL;
}

static char* treeToString(BTree tree){
    if(tree == NULL){
        return "";
    } else {
        char* result = malloc(sizeof(1024));
        char* left = treeToString(tree->left);
        char* right = treeToString(tree->right);
        strncat(result, left, 1024);
        strncat(result, tree->value, 1024);
        strncat(result, right, 1024);
        return result;
    }
}

static BTree stringToTree(const char* value){
    char* split;
    char* oprs[] = {";", "&", "|", "&&", "||", "<", ">", NULL};
    char* opr;
    for(int i = 0;opr = oprs[i]; i++){
        if(split = strstr(value, opr)){ \
            *split = '\0'; \
            return newNode(opr, stringToTree(value), stringToTree(split + strlen(oprs[i]))); \
        }
    }
    return newNode(value, NULL, NULL);
}

static void execTree(BTree tree);

static void seq(BTree tree){
    if(fork()){
        wait(NULL);
        execTree(tree->right);
    } else {
        execTree(tree->left);
    }
}

static void bg(BTree tree){
    if(fork()){
        execTree(tree->right);
    } else {
        execTree(tree->left);
    }
}

static void and(BTree tree){
    if(fork()){
        int status;
        wait(&status);
        if(status == 0){
            execTree(tree->right);
        }
    } else {
        execTree(tree->left);
    }
}

static void or(BTree tree){
    if(fork()){
        int status = 1;
        wait(&status);
        if(status != 0){
            execTree(tree->right);
        }
    } else {
        execTree(tree->left);
    }
}

static void pip(BTree tree){
    int p[2];
    pipe(p);
    if(fork()){
        wait(NULL);
        close(p[1]);
        dup2(p[0], 0);
        close(p[0]);
        execTree(tree->right);
    } else {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        execTree(tree->left);
    }
}

static void in(BTree tree){
    int f = open(tree->right->value, O_RDONLY);
    dup2(f, 0);
    close(f);
    execTree(tree->left);
}

static void out(BTree tree){
    int f = open(tree->right->value, O_CREAT | O_WRONLY, 0644);
    dup2(f, 1);
    close(f);
    execTree(tree->left);
}

/*
Recebe array de comando e argumentos

Devolve array com tudo separado
*/
char **cmdArgs(char *cmd){
	char *buff[128];
	int i = 0;
	char *str = strdup(cmd);

	while(*str == ' '){
		str++;
	}

	char *tok = strtok(str, " ");
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

static void execTree(BTree tree){
    char* oprs[] = {";", "&", "&&", "||", "|", "<", ">", NULL};
    void (*funcs[])(BTree) = {seq, bg, and, or, pip, in, out, NULL};
    if(isOperator(tree)){
        for(int i = 0; oprs[i]; i++){
            if(strcmp(tree->value, oprs[i]) == 0){
                funcs[i](tree);
            }
        }
    } else {
		char **args;
		args = cmdArgs(tree->value);

		execvp(args[0], args);
    }
}

void execute(const char* string){
	BTree tree = stringToTree(string);
    if(fork()){
        wait(NULL);
    } else {
        execTree(tree);
        exit(0);
    }
}