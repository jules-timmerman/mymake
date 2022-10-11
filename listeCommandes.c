#include "listeCommandes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


listeCommandes_t* createListeCommands(void){
	return NULL;
}


// Booléen pour si on libère l'intérieur ou pas
void freeListeCommands(listeCommandes_t* c, int freeContent){ 
	if(c != NULL){
		listeCommandes_t* next = c->next;
		if(freeContent == 1){
			free(c->commande); // Cf la création on doit copier
		}
		free(c);
		freeListeCommands(next, freeContent);
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

listeCommandes_t* revListCommande(listeCommandes_t* list){
	if (list == NULL){
		return NULL;
	}
	else{
		listeCommandes_t* nouvelleListe = malloc(sizeof(listeCommandes_t));
		nouvelleListe->next = NULL;
		nouvelleListe->commande = list->commande;
		listeCommandes_t* acc = list->next;
		while (acc != NULL){
			nouvelleListe = addCommande(nouvelleListe, acc->commande); // On ajoute la suivante en tête
			acc = acc->next; // On regarde la suite
		}
		freeListeCommands(list, 0); // On a crée une nouvelle liste donc on free l'ancienne mais pas le contenu
		return nouvelleListe;
	};
}


void execCommandes(listeCommandes_t* list){
	if(list == NULL){ // Bout de liste, cas d'arrêt
		return;
	}
	printf("%s\n", list->commande);
	system(list->commande);
	execCommandes(list->next);
}