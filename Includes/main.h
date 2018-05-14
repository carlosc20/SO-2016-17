#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.h"
#include "struct.h"

void print_CMD(char *cmds[]);
char **cmdArgs(char *cmd);
void execCMD(char *str);
void callCMDS(DynaArray *cmds);

#endif