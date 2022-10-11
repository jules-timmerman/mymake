#ifndef REGLE_H
#define REGLE_H

#include "listeCommandes.h"
#include <sys/types.h> // Pour time_t
#include <stdio.h>

struct regle{
	char* nom; // Nom de la règle
	char** prerequis; // Tableau des prérequis
	int lenPrerequis; // Taille du tableau
	listeCommandes_t* commandes; // Liste des commandes à faire
	time_t lastModified; // Heure de dernière modification (en secondes)
	int hashModified; // Booléen : 1 si le fichier [nom] modifié depuis la dernière compilation (pour le hash)
};

typedef struct regle regle_t; // Raccourci du type

regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes);
void freeRegle(regle_t* r, int isPseudo); // Le booléen nous permet de savoir s'il s'agit d'une pseudo-règle ou pas

regle_t* createRegleHash(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes);

void updateLastModified(regle_t* r);
time_t getLastModified(char* file); // Récupère la date de modification du fichier
int isFile(char* nom); // Vérifie si la règle réfère à un fichier

int hashWasModified(char* nom); // Vérifie s'il y a eu une modification du hash depuis la dernière fois
void updateHash(char* nom, unsigned long hash, FILE* f); // Modifie le nouveau hash


#endif