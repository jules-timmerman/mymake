#ifndef REGLE_H
#define REGLE_H

struct regle{
	char* nom;
	char** prerequis;
	int lenPrerequis;
	listeCommandes commandes;
	int lenCommandes;
	time_t lastModified; // La dernière modification (en secondes)
};

typedef struct regle regle_t; // Creation de la regle

regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, char** commandes, int lenCommandes);
void freeRegle(regle_t* r);

time_t getLastModified(char* file); // Récupère la date de modification du fichier
int estFichier(regle_t* regle); // Vérifie si la règle réfère à un fichier

#endif