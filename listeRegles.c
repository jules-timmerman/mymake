#include "listeRegles.h"
#include <stdlib.h>

// Une liste de règles est soit le pointeur NULL,
// soit un couple composé d'une règle et d'un pointeur vers une liste de règles
struct listeRegles{
	regle_t* regle;
	listeRegles_t* next; // NULL quand le dernier
};

// Ajoute une règle [r] à la liste de règles [list]
listeRegles_t* addRegle(listeRegles_t* list, regle_t* r){
	listeRegles_t* retList = malloc(sizeof(listeRegles_t));
	retList->regle = r;
	retList->next = list;
	return retList;
}

// Renvoie un pointeur vers la règle de nom [nom], et NULL si une telle règle n'existe pas
regle_t* rechercheRegle(listeRegles_t* list, char nom){
	if (list == NULL){
		return NULL;
	}
	else {
		if (*(list->regle->nom) == nom) {
		return list->regle;
	}
	else {
		rechercheRegle(list->next, nom);
	}
	}
}