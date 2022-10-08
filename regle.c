#include "regle.h"

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes){
	regle_t* r = malloc(sizeof(regle_t));
	r->nom = nom;
	r->prerequis = prerequis;
	r->commandes = commandes;
	r->lastModified = getLastModified(nom);	
	return r;
}

regle_t* createRegleHash(char* nom, char** prerequis, int lenPrerequis, char** commandes, int lenCommandes){
	regle_t* r = malloc(sizeof(regle_t));
	r->nom = nom;
	r->prerequis = prerequis;
	r->commandes = commandes;
	r->hashModified = hashWasModified(nom);
	return r;
}

void freeRegle(regle_t* r){
	free(r);
}

time_t getLastModified(char* file){
	struct stat *stats = malloc(sizeof(struct stat)); 
	stat(file, stats);

	time_t t = stats->st_mtim.tv_sec;	

	free(stats);

	return t;
}


// TODO : optimisation en lisant par la fin ?
// 1 : fichier (.h / .c)
// 0 sinon
int estFichier(char* nom){
	if(strstr(nom, ".h") != NULL){ // On a trouvé
		return 1;
	}else if(strstr(nom, ".c") != NULL){
		return 1;
	}
	return 0;
}


// Cherche dans le fichier .hash
// Format de .hash : nom hash
int hashWasModified(char* nom){
	FILE* f = fopen(".hash", "r");
	if(f == NULL){
		printf("Couldn't open .hash (no file or problem ?)");
		return 1; // Renvoie 1 : dans le doute, on va recompiler 
	}

	char* line = NULL;
	size_t n = 0;


	while(getline(line, &n, f) != -1){ // On lit jusqu'à l'EOF dans le pire des cas
		char* s = strtok(line, " ");
		if(s == nom){ // La règle correspondante
			char* h = strotk(NULL, " "); // Lit le hash de la dernière compil
			unsigned long hAsL = strtoul(h, NULL, 0); // On convertit en hash
			unsigned long currentHash = hashFile(nom); // Calcule le hash actuel
			fclose(f); // On ferme les ressources
			return hAsL != currentHash; // On compare les hashs
		}
	}

	fclose(f);

	return 1; // On a jamais trouvé => nouveau fichier (ou dans le doute on recompil)
}