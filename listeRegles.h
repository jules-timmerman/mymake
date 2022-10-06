#ifndef LISTEREGLES_H
#define LISTEREGLES_H

#include "regle.h"

typedef struct listeRegles listeRegles_t;

listeRegles_t* addRegle(listeRegles_t* list, regle_t* r); // Ajoute en tête et renvoit une liste (cf CAML ::)

regle_t* rechercheRegle(listeRegles_t* list, char nom);

#endif