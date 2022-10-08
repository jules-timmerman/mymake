#include "listeCommandes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Cree une liste vide de commandes (= le pointeur NULL)
listeCommandes_t* createListeCommands(void){
	return NULL;
}

// Libere une liste de commandes (sans libérer les commandes elles-mêmes)
void freeListeCommands(listeCommandes_t* c){
	if(c != NULL){
		listeCommandes_t* next = c->next;
		free(c);
		freeListeCommands(next);
	}
}

// Ajoute une commande [r] a la liste de commandes [list]
listeCommandes_t* addCommande(listeCommandes_t* list, char* c){
	listeCommandes_t* retList = malloc(sizeof(listeCommandes_t));
	retList->commande = c;
	retList->next = list;
	return retList;
}


// Execute chaque commandes dans la liste
void execCommandes(listeCommandes_t* list){
	if(list == NULL){ // Bout de liste, cas d'arrêt
		return;
	}
	system(list->commande);
	execCommandes(list->next);
}