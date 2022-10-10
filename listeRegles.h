#ifndef LISTEREGLES_H
#define LISTEREGLES_H

#include "regle.h"
#include <sys/types.h> // En soit c'est dans regle.h mais bon


typedef struct listeRegles listeRegles_t;

// Une liste de regles est soit le pointeur NULL,
// soit un couple compose d'une regle et d'un pointeur vers une liste de regles
struct listeRegles{
	regle_t* regle;
	listeRegles_t* next; // NULL quand le dernier
};



listeRegles_t* createListeRegle(void);
void freeListeRegle(listeRegles_t* list, int freeContent);
listeRegles_t* createListeRegleFromPre(listeRegles_t* list, regle_t* regle); // Cree une la liste des prerequis pour une regle

listeRegles_t* addRegle(listeRegles_t* list, regle_t* r); // Ajoute en tete et renvoie une liste (cf CAML ::)
regle_t* rechercheRegle(listeRegles_t* list, char* nom); // Recherche une certaine règle
void iterRegles(listeRegles_t* list, listeRegles_t* arg1, void (*func)(listeRegles_t*,regle_t*), int ignoreNULL); // Applique func a tout les elements de la liste avec func : (listeRegles_t*, regle_t*) -> void

time_t getLatestModify(listeRegles_t* list); // Récupère la dernière modification de la liste (le MAX)

int childModified(listeRegles_t* list); // Vérifie si un élément a été modifié (cf hash)


#endif