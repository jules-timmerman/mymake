#ifndef REGLE_H
#define REGLE_H

#include "listeCommandes.h"
#include <sys/types.h> // Pour time_t

struct regle{
	char* nom;
	char** prerequis;
	int lenPrerequis;
	listeCommandes_t* commandes;
	time_t lastModified; // La dernière modification (en secondes)
	int hashModified; // Booléen signifiant s'il y a eu une modification depuis la dernière fois (pour le hash)
};

typedef struct regle regle_t; // Creation de la regle

regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes);
void freeRegle(regle_t* r);

time_t getLastModified(char* file); // Récupère la date de modification du fichier
int estFichier(char* nom); // Vérifie si la règle réfère à un fichier

int hashWasModified(char* nom); // Vérifie s'il y a eu une modification du hash depuis la dernière fois
void updateHash(char* nom, unsigned long hash, FILE* f); // Modifie le nouveau hash


#endif