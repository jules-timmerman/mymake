#ifndef LECTUREMAKEFILE_H
#define LECTUREMAKEFILE_H

#include "listeRegles.h"
#include <stdio.h>

// Prend un pointeur vers un fichier [makefile], renvoie une listeRegles_t des règles du fichier
listeRegles_t* makefile2list(FILE* makefile);

#endif