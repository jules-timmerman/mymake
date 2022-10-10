#include "listeCommandes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


listeCommandes_t* createListeCommands(void){
	return NULL;
}


void freeListeCommands(listeCommandes_t* c){
	if(c != NULL){
		listeCommandes_t* next = c->next;
		free(c);
		freeListeCommands(next);
	}
}


listeCommandes_t* addCommande(listeCommandes_t* list, char* c){
	listeCommandes_t* retList = malloc(sizeof(listeCommandes_t));
	retList->commande = c;
	retList->next = list;
	return retList;
}

/*
listeCommandes_t* auxRevList(listeCommandes_t* list1, listeCommandes_t* list2){
		if (list1 == NULL){
			return list2;
		}
		else {
			return (auxRevList (list1->next, addCommande(list2, list1->commande)));
		}
	}


listeCommandes_t* revList(listeCommandes_t* list){
	listeCommandes_t* init = createListeCommands();
	listeCommandes_t* renverse = list;
	return renverse;
}
*/

listeCommandes_t* revList(listeCommandes_t* list){
	if (list == NULL){
		return NULL;
	}
	else{
		listeCommandes_t* nouvelleListe;
		nouvelleListe->next = NULL;
		nouvelleListe->commande = list->commande;
		listeCommandes_t* acc = list->next;
		while (acc != NULL){
			nouvelleListe->next = nouvelleListe;
			nouvelleListe->commande = acc->commande;
			acc = acc->next;
		};
		freeListeCommands(list);
		return(nouvelleListe);
	};
}


void execCommandes(listeCommandes_t* list){
	if(list == NULL){ // Bout de liste, cas d'arrêt
		return;
	}
	system(list->commande);
	execCommandes(list->next);
}