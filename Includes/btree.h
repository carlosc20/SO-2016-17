#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct BinaryTree{
    char* value;
    struct BinaryTree* left;
    struct BinaryTree* right;
}* BTree;

BTree newNode(const char* value, BTree left, BTree right);

char* treeToString(BTree tree);
BTree stringToTree(const char* value);

void execute(BTree tree);

#endif
