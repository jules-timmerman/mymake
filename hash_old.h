#ifndef HASHOLD_H
#define HASHOLD_H

#define BLOCK_SIZE 256

#include <stdio.h>

struct byteAddtion;
typedef struct byteAddition byteAddition_t;

unsigned long hashStr(unsigned char *str, int size); // Fonction de hash, cf biblio
unsigned long hashFile(char* name); // Hash un fichier entier

void sumBytes(byteAddition_t* add, unsigned char b1, unsigned char b2); // Somme 2 bytes
void sumBlocks(byteAddition_t*, char[BLOCK_SIZE], unsigned char[BLOCK_SIZE]); // Calcule la somme du buffer et du nouveau bloc

long getFileSize(FILE* f);

#endif
