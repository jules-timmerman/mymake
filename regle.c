#include "regle.h"
#include <stdlib.h>

regle_t* createRegle(char* nom, char** prerequis, char** commandes){
	regle_t* r = malloc(sizeof(regle_t));
	r->nom = nom;
	r->prerequis = prerequis;
	r->commandes = commandes;
	return r;
}

void freeRegle(regle_t* r){
	free(r);
}