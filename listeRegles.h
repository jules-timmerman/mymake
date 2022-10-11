#ifndef LISTEREGLES_H
#define LISTEREGLES_H

#include "regle.h"
#include <sys/types.h> // En soit c'est dans regle.h mais bon


typedef struct listeRegles listeRegles;

// Une liste de regles est soit le pointeur NULL,
// soit un couple compose d'une regle et d'un pointeur vers une liste de regles
struct listeRegles{
	regle* regle;
	listeRegles* next; // NULL quand le dernier
};



listeRegles* createListeRegle(void);
void freeListeRegle(listeRegles* list, int freeContent);
listeRegles* createListeRegleFromPre(listeRegles* list, regle* regle); // Cree une la liste des prerequis pour une regle

listeRegles* addRegle(listeRegles* list, regle* r); // Ajoute en tete et renvoie une liste (cf CAML ::)
regle* rechercheRegle(listeRegles* list, char* nom); // Recherche une certaine règle
void iterRegles(listeRegles* list, listeRegles* arg1, void (*func)(listeRegles*,regle*), int ignoreNULL); // Applique func a tout les elements de la liste avec func : (listeRegles*, regle*) -> void

time_t getLatestModify(listeRegles* list); // Récupère la dernière modification de la liste (le MAX)

int childModified(listeRegles* list); // Vérifie si un élément a été modifié (cf hash)

listeRegles* revListRegle(listeRegles* list); // Renverse la liste en paramètre

#endif