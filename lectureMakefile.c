#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listeRegles.h"
#include "regle.c"

listeRegles_t* makefile2list(FILE *makefile){
	char *ligne_buffer = NULL;
	size_t tailleLigne_buffer;
	ssize_t tailleLigne;

	tailleLigne = getline(&ligne_buffer, &tailleLigne_buffer, makefile); // Première ligne du fichier

	while(tailleLigne>=0){

		if (tailleLigne == 1){} //Ligne vide : juste le caractère \n
		else if (*ligne_buffer != 9) { // pas de tabulation -> nouvelle règle !
			char *token;
			token = strtok(ligne_buffer, ":"); // D'abord le nom de la règle (avant ":")
			printf("Nom nouvelleRegle : %s\n", token);
			int lenPrerequis = 0;
			regle_t* nouvelleRegle = createRegle(token, NULL, lenPrerequis, NULL, 0);
			token = strtok(NULL, " \n"); // Prérequis suivant
			while(token != NULL) {
				printf( "-> %s\n", token);
				lenPrerequis++;
				token = strtok(NULL, " "); // Suivant !
			};
			printf("Nombre prerequis : %d\n", lenPrerequis);
		}
		else { // C'est une commande !
			ligne_buffer++; // Pour négliger la tabulation, on saute une case mémoire 
			printf("Commande : %s\n", ligne_buffer);
		}
		tailleLigne = getline(&ligne_buffer, &tailleLigne_buffer, makefile); // Nouvelle ligne
	}
}

listeRegles_t* main(){
	makefile2list(fopen("Makefile", "r"));
}