#ifndef LECTUREMAKEFILE_H
#define LECTUREMAKEFILE_H

#include "listecommandes.h"



listeCommandes_t* createListeCommands(void);
void freeListeCommands(listeCommandes_t* r);

listeRegles_t* makefile2list(FILE* makefile);



#endif