#include "Includes/btree.h"

typedef struct BinaryTree{
    char* value;
    struct BinaryTree* left;
    struct BinaryTree* right;
}* BTree;

//Cria um novo nodo
static BTree newNode(const char* value, BTree left, BTree right){
    BTree newTree = malloc(sizeof(struct BinaryTree));
    newTree->value = strdup(value);
    newTree->left = left;
    newTree->right = right;
    return newTree;
}

//Dá free da árvore
static void freeTree(BTree tree){
    if(tree){
        freeTree(tree->left);
        freeTree(tree->right);
        free(tree->value);
        free(tree);
    }
}

//Verifica se o nodo é um programa
static int isProgram(BTree tree){
    return tree->left == NULL && tree->right == NULL;
}

//Verifica se o nodo é um operador
static int isOperator(BTree tree){
    return tree->left != NULL || tree->right != NULL;
}

//Retorna uma string sem os espaços iniciais
static char* skipSpaces(char* string){
    char *newString = string;
    while(*newString == ' '){
        newString++;
    }
    return newString;
}

//Converte uma string da bash numa árvore binária de operadores
static BTree stringToTree(const char* string){
    char* value = strdup(string);
    char* oprs[] = {"&&", "||", ">>", ";", "&", "|", "<", ">", NULL}; // Comandos com mais caracter primeiro
    char* split = NULL;
    char* auxSplit;
    char* opr = NULL;
    char* auxOpr;
    BTree tree;

    for(int i = 0; auxOpr = oprs[i]; i++){
        auxSplit = strstr(value, auxOpr);

        if(auxSplit && split){
            if(auxSplit < split){
                split = auxSplit;
                opr = auxOpr;
            }
        } else {
            if(auxSplit){
                split = auxSplit;
                opr = auxOpr;
            }
        }
    }
    if(split){
        *split = '\0';
        tree = newNode(opr, stringToTree(value), stringToTree(split + strlen(opr)));
    } else {
        tree = newNode(value, NULL, NULL);
    }
    free(value);
    return tree;
}

static void execTree(BTree tree);

//O comando do lado direito da árvore é executado depois do comando do lado esquerdo
static void seq(BTree tree){
    if(fork()){
        wait(NULL);
        execTree(tree->right);
    } else {
        execTree(tree->left);
    }
}

//Os comandos do lado esquerdo e direito da árvore são executados em paralelo
static void bg(BTree tree){
    if(fork()){
        execTree(tree->right);
    } else {
        execTree(tree->left);
    }
}

//Se o comando do lado esquerdo da árvore emitir um erro, o comando do lado direito não é executado
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

//Se o comando do lado esquerdo da árvore emitir um erro, o comando do lado direito é executado
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

//O resultado do lado esquerdo da árvore é passado como stdin ao lado direito da árvore
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

//O resultado do lado esquerdo da árvore recebe como stdin o ficheiro no lado direito da árvore
static void in(BTree tree){
    char* file = skipSpaces(tree->right->value);
    int f = open(file, O_RDONLY);
    dup2(f, 0);
    close(f);
    execTree(tree->left);
}

//O resultado do lado esquerdo da árvore é escrito no ficheiro no lado direito da árvore
static void out(BTree tree){
    char* file = skipSpaces(tree->right->value);
    int f = open(file, O_CREAT | O_WRONLY, 0644);
    dup2(f, 1);
    close(f);
    execTree(tree->left);
}

//O resultado do lado esquerdo da árvore é adicionado ao fim do ficheiro no lado direito da árvore
static void app(BTree tree){
    char* file = skipSpaces(tree->right->value);
    int f = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
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
	char *string = strdup(cmd);

	char *str = skipSpaces(string);

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

//Executa a arvore binária de operadores
static void execTree(BTree tree){
    char* oprs[] = {"&&", "||", ">>", ";", "&", "|", "<", ">", NULL};
    void (*funcs[])(BTree) = {and, or, app, seq, bg, pip, in, out, NULL};
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

// Executa os comandos da string
void execute(const char* string){
	BTree tree = stringToTree(string);
    if(fork()){
        wait(NULL);
        freeTree(tree);
    } else {
        execTree(tree);
        exit(0);
    }
}
