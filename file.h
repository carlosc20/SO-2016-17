#ifndef FILE
#define FILE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "struct.h"

char *readFile(char *path);
char *separateCMD(DynaArray *cmds, char *cmd);

#endif