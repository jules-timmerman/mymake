#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "regle.h"
#include "listeRegles.h"
#include "listeCommandes.h"


listeRegles_t* makefile2list(FILE* makefile){
	char *ligne_buffer = NULL; // Stockage de la ligne courante
	size_t tailleLigne_buffer;
	ssize_t tailleLigne; // Taille de la ligne courante, en comptant \n
	
	bool pushed = true; // true si la dernière règle a été incluse dans la liste de règles
	char *token;
	regle_t* nouvelleRegle = createRegle(token, NULL , 0, NULL);
	listeRegles_t* liste;
	listeCommandes_t* nouvelleListeCommandes;
	tailleLigne = getline(&ligne_buffer, &tailleLigne_buffer, makefile);

	while(tailleLigne>=0){
		if (tailleLigne == 1){	// Ligne vide
			if (!pushed){ // On complète et ajoute la règle si ce n'est déjà fait (il peut y avoir plusieurs \n)
				nouvelleRegle->commandes = revList(nouvelleListeCommandes); // Renversée -> ordre de lecture
				liste = addRegle(liste, nouvelleRegle);
				pushed = true;
			}
			else {}
		}
		else if (*ligne_buffer != 9) { // pas de tabulation -> nouvelle règle !
			pushed = false; // Cette règle n'est pas encore dans liste
			token = strtok(ligne_buffer, ":"); // Nom de la règle (avant ":" dans la ligne)
			printf("Nom nouvelleRegle : %s\n", token);
			int lenPrerequis = 0;
			token = strtok(NULL, " "); 
			// Comptage du nombre de prérequis
			while(token != NULL) {
				lenPrerequis++;
				token = strtok(NULL, " ");
			};

			char* prerequis[lenPrerequis];
			token = strtok(ligne_buffer, ":"); // On repart au premier prérequis
			token = strtok(NULL, " ");
			for (int i=0 ; i< lenPrerequis; i++) {
				prerequis[i] = token;
				token = strtok(NULL, " "); // Suivant !
			};
			nouvelleRegle->lenPrerequis = lenPrerequis;
			nouvelleRegle->prerequis = prerequis;
			
			printf("Nombre prerequis : %d\n", lenPrerequis);
		}
		else { // C'est une ligne de commande !
			ligne_buffer++; // Pour négliger la tabulation, on saute une case mémoire (surement optionnel)
			printf("Commande : %s\n", ligne_buffer);
			nouvelleListeCommandes = addCommande(nouvelleListeCommandes, ligne_buffer);
		}

		tailleLigne = getline(&ligne_buffer, &tailleLigne_buffer, makefile); // Lecture nouvelle ligne
	}

	if (!pushed){ // Il reste probablement une commande à ajouter à la liste
		nouvelleRegle->commandes = revList(nouvelleListeCommandes);
		liste = addRegle(liste, nouvelleRegle);
	}
	else {}
	return liste;
}


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

void main(int argc, char** argv){
	if(argc <= 1){ // On a pas donné d'argument
		printf("Usage : mymake <cible>");
		return 1;
	}else{
		char* cible = argv[1];
		FILE* makefile = fopen("Makefile", "r");
		if(makefile == NULL){
			printf("Impossible d'ouvrir le fichier Makefile");
			return 1;
		}
		listeRegles_t* list = makefile2list(makefile);
		make_naive(list, rechercheRegle(list, cible));
	}
	// FINIR LES FREE

}