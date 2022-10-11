#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "regle.h"
#include "listeRegles.h"
#include "listeCommandes.h"


listeRegles* makefile2list(FILE* makefile){
	char *ligne_buffer = NULL; // Stockage de la ligne courante
	size_t tailleLigne_buffer = 0;
	ssize_t tailleLigne = 0; // Taille de la ligne courante, en comptant \n
	
	int pushed = 1; // bool : 1 si dernière règle a été incluse dans liste de règles
	char *token;
	regle* nouvelleRegle = NULL;
	listeRegles* liste = createListeRegle();
	listeCommandes* nouvelleListeCommandes = createListeCommands();
	tailleLigne = getline(&ligne_buffer, &tailleLigne_buffer, makefile);

	while(tailleLigne>=0){
		if (tailleLigne == 1){	// Ligne vide
			if (!pushed){ // On complète et ajoute la règle si ce n'est déjà fait (il peut y avoir plusieurs \n)
				nouvelleRegle->commandes = revListCommande(nouvelleListeCommandes); // Renversée -> ordre de lecture
				liste = addRegle(liste, nouvelleRegle);
				pushed = 1;
			}
			else {}
		}
		else if (*ligne_buffer != 9) { // pas de tabulation -> nouvelle règle !
			pushed = 0; // Cette règle n'est pas encore dans liste
			
			char* copyLigne = malloc(strlen(ligne_buffer) + 1); // On crée une copie car strtok change la chaîne et on veut faire 2 parcours
			char* memCopyLigne = copyLigne; // On garde en mémoire le début pour pouvoir free à la fin
			strcpy(copyLigne, ligne_buffer);

			token = strtok(copyLigne, ":"); // Nom de la règle (avant ":" dans la ligne)
			char* nom = malloc(sizeof(strlen(token)) + 1); // On malloc pour le nom (pour garder après l'exec), +1 pour nullbyte
			strcpy(nom, token);

			printf("Nom nouvelleRegle : %s\n", token);
			int lenPrerequis = 0;
			token = strtok(NULL, " "); 
			// Comptage du nombre de prérequis
			while(token != NULL) {
				lenPrerequis++;
				token = strtok(NULL, " ");
			};

			char** prerequis = malloc(sizeof(char*) * lenPrerequis); // On doit malloc pour que ce ne soit pas détruit au retour
			
			copyLigne = memCopyLigne; // On remet au début de la copie préparée
			strcpy(copyLigne, ligne_buffer); // On refait une nouvelle copie pour ne pas changer ligne_buffer et pouvoir free à la fin
			token = strtok(copyLigne, ":"); // On repart au premier prérequis
			token = strtok(NULL, " ");

			for (int i=0 ; i< lenPrerequis - 1; i++) { // On ne va pas au bout de la lecture cf en dessous
				prerequis[i] = malloc(strlen(token) + 1); // Pour rajouter le nullbyte
				strcpy(prerequis[i], token); // On copie
				token = strtok(NULL, " "); // Suivant !
			};
			// On traite le dernier à part à cause de la lecture d'un \n final que l'on enlève ici
			size_t len = strlen(token);
			token[len-1] = '\0'; // On change le \n en un \0 (donc on l'enlève)
			prerequis[lenPrerequis - 1] = malloc(len); // Au final la taille devient 1 de main
			strcpy(prerequis[lenPrerequis - 1], token);
			token = strtok(NULL, " "); 

			nouvelleListeCommandes = createListeCommands(); // On crée notre liste vide de commandes 
			nouvelleRegle = createRegle(nom, prerequis, lenPrerequis, nouvelleListeCommandes); // On crée la règle. On remplira la liste de commandes au fur et à mesure

			printf("Nombre prerequis : %d\n", lenPrerequis);

			free(memCopyLigne); // On free
		}
		else { // C'est une ligne de commande !
			ligne_buffer++; // Pour négliger la tabulation, on saute une case mémoire (surement optionnel)
			printf("Commande : %s\n", ligne_buffer);

			char* commande = malloc(strlen(ligne_buffer) + 1); // On fait la copie pour les mêmes raisons qu'avant (pas mourir à la fin)
			strcpy(commande, ligne_buffer);

			nouvelleListeCommandes = addCommande(nouvelleListeCommandes, commande);
			ligne_buffer--; // On redécale pour que le free fonctionne
		}

		tailleLigne = getline(&ligne_buffer, &tailleLigne_buffer, makefile); // Lecture nouvelle ligne
	}

	if (!pushed){ // Il reste probablement une commande à ajouter à la liste
		nouvelleRegle->commandes = revListCommande(nouvelleListeCommandes);
		liste = addRegle(liste, nouvelleRegle);
	}
	else {}

	free(ligne_buffer); // Cf docu getline : on doit free

	liste = revListRegle(liste); // On retourne notre liste de règle pour être dans le bon sens

	return liste;
}


// TODO : Optimisation sans iter ?
// Q6 : version naive
void make_naive(listeRegles_t* list, regle_t* regle){
	if(isFile(regle->nom) == 1){ // Cas de base
		return; // Rien à faire
	}
	// Sinon : on construit récursivement
	listeRegles* listPre = createListeRegleFromPre(list, regle);
	iterRegles(listPre, list, &make_naive, 1); // On applique make_naive à tout les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	freeListeRegle(listPre, 0); // On a fini avec cette liste

	// On compile notre règle
	execCommandes(regle->commandes);
}


// TODO : idem Q6
// Q7 : la bonne version 
void make(listeRegles_t* list, regle_t* regle){
	if(isFile(regle->nom) == 1){ // Cas de base
		return; // Rien à faire
	}

	// On récupère les listes des dépendances (en tant que règles et non de char*)
	listeRegles* listPre = createListeRegleFromPre(list, regle);
	
	// On construit récursivement les dépendances
	// On applique make à tout les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	iterRegles(listPre, list, &make, 1); 


	time_t lastChildModif = getLatestModify(listPre); // On récupère la dernière modification des enfants
	if(lastChildModif > regle->lastModified){ // On a modifié une dépendance plus récemment : on recompile
		// On compile notre règle
		execCommandes(regle->commandes);
		updateLastModified(regle); // On met à jour la dernière modification pour les parents
	}

	freeListeRegle(listPre, 0); // On free ce qu'on a utilisé
}

// Remarque : on compile d'abord les enfants donc lors d'une compilation, on aura toujours le temps modifié des enfants inférieurs au parent
// => Pas besoin d'une tolérance lors d'une même compil


// Make with hash
// Version utilisant le hash et non la date de modification
void makeWHash(listeRegles_t* list, regle_t* regle){
	if(isFile(regle->nom) == 1){ // Cas de base
		return; // Rien à faire
	}

	// On récupère les listes des dépendances (en tant que règles et non de char*)
	listeRegles* listPre = createListeRegleFromPre(list, regle);
	
	// On construit récursivement les dépendances
	// On applique makeWHash à tous les éléments de listPre (avec argument list) (en ignorant NULL au cas où)
	iterRegles(listPre, list, &makeWHash, 1); 


	if(childModified(listPre) == 1){ // On a modifié une dépendance plus récemment : on recompile
		// On compile notre règle
		execCommandes(regle->commandes);
	}

	

	freeListeRegle(listPre, 0); // On free ce qu'on a utilisé (ainsi que les pseudos-règles au passage)
}


int main(int argc, char** argv){
		FILE* makefile = fopen("Makefile", "r");
		if(makefile == NULL){
			printf("Impossible d'ouvrir le fichier Makefile");
			return 1;
		}
		listeRegles* list = makefile2list(makefile);
		char* cible;
		if(argc <= 1){ // On a pas donné d'argument
			cible = list->regle->nom;
		}
		else {
			cible = argv[1];
		}
		make(list, rechercheRegle(list, cible));
		
		freeListeRegle(list, 1);
		fclose(makefile);
}