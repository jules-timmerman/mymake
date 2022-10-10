#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listeRegles.h"


// Cree une liste vide de regles (= le pointeur NULL)
listeRegles_t* createListeRegle(void){
	return NULL;
}

// Libere une liste de regles (sans libérer les règles elles-mêmes)
void freeListeRegle(listeRegles_t* list){
	if(list != NULL){
		listeRegles_t* next = list->next;
		
		if(estFichier(list->regle->nom) == 1){ // Il s'agit d'une pseudo-règle (cf createListeRegleFromPre)
			freeRegle(list->regle);
		}
		
		free(list);
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

// Cree la liste des règles prérequises pour une règle donnée (pour les .h/.c, on crée des pseudos-règles)
listeRegles_t* createListeRegleFromPre(listeRegles_t* list, regle_t* regle){
	listeRegles_t* retList = createListeRegle(); // Liste vide que l'on retournera

	for(int i = 0; i < regle->lenPrerequis; i++){
		if(estFichier(regle->prerequis[i]) == 1){ // Les .h / .c n'ont pas de règles dans list : on crée une pseudo règle pour faciliter l'écriture du programme
			// On freera la pseudo-règle en même temps que le retour de la fonction
			// On passe par createRegle pour pouvoir bien initialiser le hash etc...
			regle_t* r = createRegle(regle->prerequis[i], NULL, 0, NULL); 

			addRegle(retList, r);
		}else{
			regle_t* r = rechercheRegle(list, *regle->prerequis[i]); // On cherche le ieme prerequis dans la liste
			if(r != NULL){ // Si on a trouvé une règle correspondant au ieme prérequis
				addRegle(retList, r); // On l'ajoute à notre liste de retour
			}
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
	if(recT < list->regle->lastModified){ return list->regle->lastModified; } // On renvoie le max
	return recT;
}


int childModified(listeRegles_t* list){
	if(list == NULL){ // Cas de base : on est arrivé au bout
		return 0;
	}
	if(list->regle->hashModified == 1){ // La règle a été modifiée : on termine directement
		return 1;
	}
	return childModified(list->next);
}

