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
void execCMD(char *cmds[], int fd);

#endif