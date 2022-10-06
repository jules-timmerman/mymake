#include "listeRegles.h"
#include <stdlib.h>

struct listeRegles{
	regle_t* regle;
	listeRegles_t* next;
};


regle_t* rechercheRegle(listeRegles_t* list, char nom){
	if (list == NULL){
		return NULL;
	}
	else if (list->regle->nom == nom) {
		return list->regle;
	}
	else {
		rechercheRegle(list->next, nom);
	}
}