#ifndef REGLE_H
#define REGLE_H

#include "listeCommandes.h"
#include <sys/types.h> // Pour time_t
#include <stdio.h>

struct regle{
	char* nom; // Nom de la règle
	char** prerequis; // Tableau des prérequis
	int lenPrerequis; // Taille du tableau
	listeCommandes* commandes; // Liste des commandes à faire
	time_t lastModified; // Heure de dernière modification (en secondes)
	int hashModified; // Booléen : 1 si le fichier [nom] modifié depuis la dernière compilation (pour le hash)
};

typedef struct regle regle; // Creation de la regle

regle* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes* commandes);
void freeRegle(regle* r, int isPseudo); // Le booléen nous permet de savoir s'il s'agit d'une pseudo-règle ou pas

regle* createRegleHash(char* nom, char** prerequis, int lenPrerequis, listeCommandes* commandes);

void updateLastModified(regle* r);
time_t getLastModified(char* file); // Récupère la date de modification du fichier
int isFile(char* nom); // Vérifie si la règle réfère à un fichier

int hashWasModified(char* nom); // Vérifie s'il y a eu une modification du hash depuis la dernière fois
void updateHash(char* prettyHash, FILE* f); // Modifie le nouveau hash
void addHash(char* nom, char* prettyHash, FILE* f); // Ajoute un nouveau hash au fichier .hash

void hashFile(char* nom, unsigned char* hash);

#endif