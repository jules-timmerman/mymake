#include "listeRegles.h"
#include <stdlib.h>

struct listeRegles{
	regle_t* regle;
	listeRegles_t* next; // NULL quand le dernier
};

listeRegles_t* addRegle(listeRegles_t* list, regle_t* r){
	listeRegles_t* retList = malloc(sizeof(listeRegles_t));
	retList->regle = r;
	retList->next = list;
	return retList;
}