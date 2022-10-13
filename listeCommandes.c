#include "listeCommandes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


listeCommandes* createListeCommands(void){
	return NULL;
}


// Booléen pour si on libère l'intérieur ou pas
void freeListeCommands(listeCommandes* c, int freeContent){ 
	if(c != NULL){
		listeCommandes* next = c->next;
		if(freeContent == 1){
			free(c->commande); // Cf la création on doit copier
		}
		free(c);
		freeListeCommands(next, freeContent);
	}
}



listeCommandes* addCommande(listeCommandes* list, char* c){
	listeCommandes* retList = malloc(sizeof(listeCommandes));
	retList->commande = c;
	retList->next = list;
	return retList;
}

/*
listeCommandes* auxRevList(listeCommandes* list1, listeCommandes* list2){
		if (list1 == NULL){
			return list2;
		}
		else {
			return (auxRevList (list1->next, addCommande(list2, list1->commande)));
		}
	}


listeCommandes* revList(listeCommandes* list){
	listeCommandes* init = createListeCommands();
	listeCommandes* renverse = list;
	return renverse;
}
*/

listeCommandes* revListCommande(listeCommandes* list){
	if (list == NULL){
		return NULL;
	}
	else{
		listeCommandes* nouvelleListe = malloc(sizeof(listeCommandes));
		nouvelleListe->next = NULL;
		nouvelleListe->commande = list->commande;
		listeCommandes* acc = list->next;
		while (acc != NULL){
			nouvelleListe = addCommande(nouvelleListe, acc->commande); // On ajoute la suivante en tête
			acc = acc->next; // On regarde la suite
		}
		freeListeCommands(list, 0); // On a crée une nouvelle liste donc on free l'ancienne mais pas le contenu
		return nouvelleListe;
	};
}


void execCommandes(listeCommandes* list){
	if(list == NULL){ // Bout de liste, cas d'arrêt
		return;
	}
	printf("%s", list->commande);
	system(list->commande);
	execCommandes(list->next);
}