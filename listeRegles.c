#include "listeRegles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Cree une liste vide de regles (= le pointeur NULL)
listeRegles_t* createListeRegle(void){
	return NULL;
}

// Libere une liste de regles (sans libérer les règles elles-mêmes)
void freeListeRegle(listeRegles_t* r){
	if(r != NULL){
		listeRegles_t* next = r->next;
		free(r);
		freeListeRegle(next);
	}
}

// Ajoute une regle [r] a la liste de regles [list]
listeRegles_t* addRegle(listeRegles_t* list, regle_t* r){
	listeRegles_t* retList = malloc(sizeof(listeRegles_t));
	retList->regle = r;
	retList->next = list;
	return retList;
}

// Cree la liste des règles prérequises pour une règle donnée
listeRegles_t* createListeRegleFromPre(listeRegles_t* list, regle_t* regle){
	listeRegles_t* retList = createListeRegle(); 
	for(int i = 0; i < regle->lenPrerequis; i++){
		regle_t* r = rechercheRegle(list, regle->prerequis[i]);
		if(r != NULL){ // Si on a trouvé une règle correspondant au ieme prérequis
			addRegle(retList, r); // On l'ajoute à notre liste de retour
		}
	}
	return retList;
}


// Renvoie un pointeur vers la regle nommée [nom], et NULL si une telle regle n'existe pas
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

// Itère sur la fonction
// si ignoreNULL = 1, on applique ignore les regles NULL
void iterRegles(listeRegles_t* regles, listeRegles_t* arg1, void (*func)(listeRegles_t*, regle_t*), int ignoreNULL){
	if(regles == NULL){ // Cas d'arrêt : on est sur la liste vide
		return;
	}
	if(regles->regle != NULL || ignoreNULL == 0){ // On applique que quand regle != NULL ou (ignoreNULL = 0 et regles = NULL) 
		(*func) (arg1, regles->regle);
	}
	iterRegles(regles->next, arg1, func, ignoreNULL);
}

time_t getLatestModify(listeRegles_t* list){
	if(list == NULL){ // Cas de base : liste vide
		return 0;
	}
	time_t recT = getLatestModify(list->next); // Appel récursif
	if(recT < list->regle->lastModified){ return list->regle->lastModified; } // On renvoit le max
	return recT;
}