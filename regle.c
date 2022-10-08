#include "regle.h"
#include <stdlib.h>

#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>


regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes){
	regle_t* r = malloc(sizeof(regle_t));
	r->nom = nom;
	r->prerequis = prerequis;
	r->commandes = commandes;
	r->lastModified = getLastModified(nom);	
	return r;
}

void freeRegle(regle_t* r){
	free(r);
}

time_t getLastModified(char* file){
	struct stat *stats = malloc(sizeof(struct stat)); 
	stat(file, stats);

	time_t t = stats->st_mtim.tv_sec;	

	free(stats);

	return t;
}


// TODO : optimisation en lisant par la fin ?
// 1 : fichier (.h / .c)
// 0 sinon
int estFichier(regle_t* regle){
	if(strstr(regle->nom, ".h") != NULL){ // On a trouvé
		return 1;
	}else if(strstr(regle->nom, ".c") != NULL){
		return 1;
	}
	return 0;
}