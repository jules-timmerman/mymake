#include "listeRegles.h"
#include <stdlib.h>

// Une liste de regles est soit le pointeur NULL,
// soit un couple compose d'une regle et d'un pointeur vers une liste de regles
struct listeRegles{
	regle_t* regle;
	listeRegles_t* next; // NULL quand le dernier
};

// Cree une liste vide de regles (= le pointeur NULL)
listeRegles_t* createListeRegle(void){
	return NULL;
}
// Libere une liste de regles
void freeListeRegle(listeRegles_t* r){
	free(r);
}

// Ajoute une regle [r] à la liste de regles [list]
listeRegles_t* addRegle(listeRegles_t* list, regle_t* r){
	listeRegles_t* retList = malloc(sizeof(listeRegles_t));
	retList->regle = r;
	retList->next = list;
	return retList;
}

// Renvoie un pointeur vers la regle de nom [nom], et NULL si une telle regle n'existe pas
regle_t* rechercheRegle(listeRegles_t* list, char nom){
	if (list == NULL){
		return NULL;
	}
	else if (*(list->regle->nom) == nom) {
		return list->regle;
	}
	else {
		rechercheRegle(list->next, nom);
	}
	
}