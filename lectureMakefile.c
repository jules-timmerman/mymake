#include "lectureMakefile.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "listeCommandes.h"


listeRegles_t* makefile2list(FILE* makefile){
	char *ligne_buffer = NULL; // Stockage de la ligne courante
	size_t tailleLigne_buffer;
	ssize_t tailleLigne; // Taille de la ligne courante, en comptant \n
	
	bool pushed = true; // true si la dernière règle a été incluse dans la liste de règles
	char *token;
	regle_t* nouvelleRegle = createRegle(token, NULL , 0, NULL);
	listeRegles_t* liste = createListeRegle();
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
			char* nom = token;
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
			nouvelleRegle = createRegle(nom, prerequis, lenPrerequis, NULL);			
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
	return createListeRegle();
	//return liste;
}

//listeRegles_t* main(){
//	makefile2list(fopen("Makefile", "r"));
//}