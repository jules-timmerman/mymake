#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listeRegles.h"


// Cree une liste vide de regles (= le pointeur NULL)
listeRegles* createListeRegle(void){
	return NULL;
}

// Libere une liste de regles (sans libérer les règles elles-mêmes)
void freeListeRegle(listeRegles* list, int freeContent){ // Booléen si on libère le contenu avec ou pas
	if(list != NULL){
		listeRegles* next = list->next;
		
		if(isFile(list->regle->nom) == 1){ // Il s'agit d'une pseudo-règle (cf createListeRegleFromPre)
			freeRegle(list->regle, 1);
		}

		if(freeContent == 1){
			freeRegle(list->regle, 0);
		}
		
		free(list);
		freeListeRegle(next, freeContent);
	}
}

// Ajoute une regle [r] a la liste de regles [list]
listeRegles* addRegle(listeRegles* list, regle* r){
	listeRegles* retList = malloc(sizeof(listeRegles));
	retList->regle = r;
	retList->next = list;
	return retList;
}

// Cree la liste des règles prérequises pour une règle donnée (pour les .h/.c, on crée des pseudos-règles)
listeRegles* createListeRegleFromPre(listeRegles* list, regle* r){
	listeRegles* retList = createListeRegle(); // Liste vide que l'on retournera

	for(int i = 0; i < r->lenPrerequis; i++){
		if(isFile(r->prerequis[i]) == 1){ // Les .h / .c n'ont pas de règles dans list : on crée une pseudo règle pour faciliter l'écriture du programme
			// On freera la pseudo-règle en même temps que le retour de la fonction
			// On passe par createRegle pour pouvoir bien initialiser le hash etc...
			regle* retR = createRegle(r->prerequis[i], NULL, 0, NULL); 

			retList = addRegle(retList, retR);
		}else{
			regle* retR = rechercheRegle(list, r->prerequis[i]); // On cherche le ieme prerequis dans la liste
			if(retR != NULL){ // Si on a trouvé une règle correspondant au ieme prérequis
				retList = addRegle(retList, retR); // On l'ajoute à notre liste de retour
			}
		}
	}
	return retList;
}


// Renvoie un pointeur vers la regle nommée [nom], et NULL si une telle regle n'existe pas
regle* rechercheRegle(listeRegles* list, char* nom){
	if (list == NULL){
		return NULL;
	}
	else if (strcmp(list->regle->nom, nom) == 0) {
		return list->regle;
	}
	else {
		rechercheRegle(list->next, nom);
	}
}

// Itère sur la fonction
// si ignoreNULL = 1, on applique ignore les regles NULL
void iterRegles(listeRegles* regles, listeRegles* arg1, void (*func)(listeRegles*, regle*), int ignoreNULL){
	if(regles == NULL){ // Cas d'arrêt : on est sur la liste vide
		return;
	}
	if(regles->regle != NULL || ignoreNULL == 0){ // On applique que quand regle != NULL ou (ignoreNULL = 0 et regles = NULL) 
		(*func) (arg1, regles->regle);
	}
	iterRegles(regles->next, arg1, func, ignoreNULL);
}

time_t getLatestModify(listeRegles* list){
	if(list == NULL){ // Cas de base : liste vide
		return 0;
	}
	time_t recT = getLatestModify(list->next); // Appel récursif
	if(recT < list->regle->lastModified){ return list->regle->lastModified; } // On renvoie le max
	return recT;
}


int childModified(listeRegles* list){
	if(list == NULL){ // Cas de base : on est arrivé au bout
		return 0;
	}
	if(list->regle->hashModified == 1){ // La règle a été modifiée : on termine directement
		return 1;
	}
	return childModified(list->next);
}

listeRegles* revListRegle(listeRegles* list){
	if (list == NULL){
		return NULL;
	}
	else{
		listeRegles* nouvelleListe = malloc(sizeof(listeRegles));
		nouvelleListe->next = NULL;
		nouvelleListe->regle = list->regle;
		listeRegles* acc = list->next;
		while (acc != NULL){
			nouvelleListe = addRegle(nouvelleListe, acc->regle); // On ajoute la suivante en tête
			acc = acc->next; // On regarde la suite
		}
		freeListeRegle(list, 0); // On a crée une nouvelle liste donc on free l'ancienne mais pas le contenu
		return nouvelleListe;
	};
}

