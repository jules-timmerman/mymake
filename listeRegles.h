#ifndef LISTEREGLES_H
#define LISTEREGLES_H

#include "regle.h"

typedef struct listeRegles listeRegles_t;

void addRegle(listeRegles_t* list, regle_t* r);

regle_t* rechercheRegle(listeRegles_t* list, char nom);


#endif