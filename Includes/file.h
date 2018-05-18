#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "struct.h"

char *readFile(int fd);
void separateCMD(DynaArray *cmds, char *cmd);

#endif
