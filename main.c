#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "regle.h"
#include "listeRegles.h"


/* Transforme une regle de type main: main.o regle.o listeRegles.o */
listeRegles_t readMakefile(FILE* makefile){
    char ligne[512];
    listeRegles_t* liste = createListeRegle(); 
    while (getline(makefile, "%s", &ligne)){
        if (ligne[0] != '	') {
            char** decoupe;
            regle_t* nouvelleRegle;
            decoupe = strtok (ligne, ' :\t');
            // nouvelleRegle = createRegle(decoupe[0], decoupe, decoupe);
            liste = addRegle(liste, nouvelleRegle);
           }
    }
    return *liste;
}


// TODO : Optimisation sans iter ?
// Q6 : version naive
void make_naive(listeRegles_t* list, regle_t* regle){
	if(estFichier(regle) == 1){ // Cas de base
		return; // Rien à faire
	}
	// Sinon : on construit récursivement
	listeRegles_t* listPre = createListeRegleFromPre(list, regle);
	iterRegles(listPre, list, &make_naive, 1); // On applique buildRegleNaive à tout les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	freeListeRegle(listPre); // On a fini avec cette liste


	for(int i = 0; i < regle->lenCommandes; i++){ // On compile enfin la règle elle-même
		system(regle->commandes[i]);
	}
}




// TODO : idem Q6
// Q7 : la bonne version 
void make(listeRegles_t* list, regle_t* regle){
	if(estFichier(regle) == 1){ // Cas de base
		return; // Rien à faire
	}

	// On récupère les listes des dépendances (en tant que règles et non de char*)
	listeRegles_t* listPre = createListeRegleFromPre(list, regle);
	
	// On construit récursivement les dépendances
	// On applique buildRegleNaive à tout les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	iterRegles(listPre, list, &make, 1); 


	time_t lastChildModif = getLatestModify(listPre); // On récupère la dernière modification des enfants
	if(lastChildModif > regle->lastModified){ // On a modifié une dépendance plus récemment : on recompile
		for(int i = 0; i < regle->lenCommandes; i++){ // On compile la règle
			system(regle->commandes[i]);
		}
	}

	freeListeRegle(listPre); // On free ce qu'on a utilisé
}

// Remarque : on compile d'abord les enfants donc lors d'une compilation, on aura toujours le temps modifié des enfants inférieurs au parent
// => Pas besoin d'une tolérance lors d'une même compil


// TODO : Pas oublier de free les règles quelques part (soit à la fin de make, soit dans main)

void main(){
    printf("Hello, world.");
}