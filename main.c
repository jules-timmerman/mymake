#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "regle.h"
#include "listeRegles.h"



// TODO : Optimisation sans iter ?
// Q6 : version naive
void make_naive(listeRegles_t* list, regle_t* regle){
	if(estFichier(regle->nom) == 1){ // Cas de base
		return; // Rien à faire
	}
	// Sinon : on construit récursivement
	listeRegles_t* listPre = createListeRegleFromPre(list, regle);
	iterRegles(listPre, list, &make_naive, 1); // On applique make_naive à tout les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	freeListeRegle(listPre); // On a fini avec cette liste


	// On compile notre règle
	execCommandes(regle->commandes);
}


// TODO : idem Q6
// Q7 : la bonne version 
void make(listeRegles_t* list, regle_t* regle){
	if(estFichier(regle->nom) == 1){ // Cas de base
		return; // Rien à faire
	}

	// On récupère les listes des dépendances (en tant que règles et non de char*)
	listeRegles_t* listPre = createListeRegleFromPre(list, regle);
	
	// On construit récursivement les dépendances
	// On applique make à tout les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	iterRegles(listPre, list, &make, 1); 


	time_t lastChildModif = getLatestModify(listPre); // On récupère la dernière modification des enfants
	if(lastChildModif > regle->lastModified){ // On a modifié une dépendance plus récemment : on recompile
		// On compile notre règle
		execCommandes(regle->commandes);
	}

	freeListeRegle(listPre); // On free ce qu'on a utilisé
}

// Remarque : on compile d'abord les enfants donc lors d'une compilation, on aura toujours le temps modifié des enfants inférieurs au parent
// => Pas besoin d'une tolérance lors d'une même compil


// Make with hash
// Version utilisant le hash et non la date de modification
void makeWHash(listeRegles_t* list, regle_t* regle){
	if(estFichier(regle->nom) == 1){ // Cas de base
		return; // Rien à faire
	}

	// On récupère les listes des dépendances (en tant que règles et non de char*)
	listeRegles_t* listPre = createListeRegleFromPre(list, regle);
	
	// On construit récursivement les dépendances
	// On applique makeWHash à tous les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	iterRegles(listPre, list, &makeWHash, 1); 


	if(childModified(listPre) == 1){ // On a modifié une dépendance plus récemment : on recompile
		// On compile notre règle
		execCommandes(regle->commandes);
	}


	freeListeRegle(listPre); // On free ce qu'on a utilisé (ainsi que les pseudos-règles au passage)
}





// TODO : Pas oublier de free les règles quelques part (soit à la fin de make, soit dans main)

void main(){
    printf("Hello, world.");
}