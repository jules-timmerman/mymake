#ifndef LECTUREMAKEFILE_H
#define LECTUREMAKEFILE_H

#include "regle.h"
#include "listeRegles.h"
// Utile ???
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct listeCommandes listeCommandes_t;

// Une liste de commandes est soit le pointeur NULL,
// soit un couple composé d'une commande et d'un pointeur vers une liste de commandes
struct listeCommandes{
	char* commande;
	listeCommandes_t* next; // NULL quand le dernier
};


listeCommandes_t* createListeCommands(void);
void freeListeCommands(listeCommandes_t* r);

listeRegles_t* makefile2list(FILE* makefile);



#endif