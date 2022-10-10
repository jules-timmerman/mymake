#include "regle.h"

#include "hash.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes){
	regle_t* r = malloc(sizeof(regle_t));
	r->nom = nom;
	r->prerequis = prerequis;
	r->lenPrerequis = lenPrerequis;
	r->commandes = commandes;
	r->lastModified = getLastModified(nom);	
	return r;
}

regle_t* createRegleHash(char* nom, char** prerequis, int lenPrerequis, listeCommandes_t* commandes){
	regle_t* r = malloc(sizeof(regle_t));
	r->nom = nom;
	r->prerequis = prerequis;
	r->lenPrerequis = lenPrerequis;
	r->commandes = commandes;
	r->hashModified = hashWasModified(nom);
	return r;
}

void freeRegle(regle_t* r, int isPseudo){
	if(isPseudo == 0){
		free(r->nom); // On libère le nom
		
		for(int i = 0; i < r->lenPrerequis; i++){ // Chaque prérequis
			free(r->prerequis[i]);
		}
		free(r->prerequis);	// Le tableau lui-même
		
		freeListeCommands(r->commandes);	
	}

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
	FILE* f = fopen(".hash", "rw");
	if(f == NULL){
		printf("Couldn't open .hash (no file or problem ?)");
		return 1; // Renvoie 1 : dans le doute, on va recompiler 
	}

	char* line = NULL;
	size_t n = 0;


	while(getline(&line, &n, f) != -1){ // On lit jusqu'à l'EOF dans le pire des cas
		char* s = strtok(line, " ");
		if(s == nom){ // La règle correspondante
			char* h = strtok(NULL, " "); // Lit le hash de la dernière compil
			unsigned long hAsL = strtoul(h, NULL, 0); // On convertit en hash
			unsigned long currentHash = hashFile(nom); // Calcule le hash actuel
			if(hAsL != currentHash){ // Le hash a été changé : on le met à jour dans .hash
				updateHash(nom, currentHash, f);
			}
			fclose(f); // On ferme les ressources
			return hAsL != currentHash; // On compare les hashs
		}
	}

	// On arrive ici : on a jamais trouvé dans le ficher : on ajoute à la fin 


	fclose(f);

	return 1; // On a jamais trouvé => nouveau fichier (ou dans le doute on recompil)
}

void updateHash(char* nom, unsigned long hash, FILE* f){ // Modifie le nouveau hash
	// TODO : faire la fonction ducon


}