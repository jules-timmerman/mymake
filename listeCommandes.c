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

// Renverse la liste [list1] dans la liste [list2]
listeCommandes_t* auxRevList(listeCommandes_t* list1, listeCommandes_t* list2){
		if (list1 == NULL){
			return list2;
		}
		else {
			return (auxRevList (list1->next, addCommande(list2, list1->commande)));
		}
	}

// Renvoie un pointeur vers la liste renversée de [list]
listeCommandes_t* revList(listeCommandes_t* list){
	return (auxRevList(list, createListeCommands()));
}


// Execute chaque commandes dans la liste
void execCommandes(listeCommandes_t* list){
	if(list == NULL){ // Bout de liste, cas d'arrêt
		return;
	}
	system(list->commande);
	execCommandes(list->next);
}