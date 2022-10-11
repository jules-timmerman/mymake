#include "regle.h"

#include "hash.h"
#include "sha256.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


regle* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes* commandes){
	regle* r = malloc(sizeof(regle));
	r->nom = nom;
	r->prerequis = prerequis;
	r->lenPrerequis = lenPrerequis;
	r->commandes = commandes;
	r->lastModified = getLastModified(nom);	
	return r;
}

regle* createRegleHash(char* nom, char** prerequis, int lenPrerequis, listeCommandes* commandes){
	regle* r = malloc(sizeof(regle));
	r->nom = nom;
	r->prerequis = prerequis;
	r->lenPrerequis = lenPrerequis;
	r->commandes = commandes;
	r->hashModified = hashWasModified(nom);
	return r;
}

void freeRegle(regle* r, int isPseudo){
	if(isPseudo == 0){
		free(r->nom); // On libère le nom
		for(int i = 0; i < r->lenPrerequis; i++){ // Chaque prérequis
			free(r->prerequis[i]);
		}
		free(r->prerequis);	// Le tableau lui-même
		freeListeCommands(r->commandes, 1); // Ici on free le contenu avec (c'est la fin)	
	}
	free(r);
}

void updateLastModified(regle* r){
	r->lastModified = getLastModified(r->nom);
}

time_t getLastModified(char* file){
	struct stat *stats = malloc(sizeof(struct stat)); 
	stat(file, stats);

	time_t t = stats->st_mtim.tv_sec;	

	free(stats);

	return t;
}

// Test si extension fichier en (.h / .c)
int isFile(char* nom){
	int length = strlen(nom);
	if (length<2){
		return 0;
	}
	return (nom[length-2] == '.' && (nom[length-1] == 'c' || nom[length-1] == 'h'));
}

// Cherche dans le fichier .hash
// Format de .hash : nom hash
int hashWasModified(char* nom){
	FILE* f = fopen(".hash", "r+"); // r+ lecture et écriture depuis le début du fichier sans écraser tout.
	if(f == NULL){
		printf("Couldn't open .hash (no file or problem ?)");
		return 1; // Renvoie 1 : dans le doute, on va recompiler 
	}

	unsigned char currentHash[SHA256_BLOCK_SIZE + 1];
	currentHash[SHA256_BLOCK_SIZE] = '\0'; // On rajoute le \0 pour pouvoir utiliser la chaîne ensuite (dans strcmp etc...) (j'imagine que sha ne le rajoute pas)
	hashFile(nom, currentHash); // Calcule le hash actuel


	char* line = NULL;
	size_t n = 0;

	while(getline(&line, &n, f) != -1){ // On lit jusqu'à l'EOF dans le pire des cas à la recherche de notre nom (le fichier)
		char* s = strtok(line, " ");
		if(strcmp(s, nom) == 0){ // La règle correspondante (on a trouvé)
			char* h = strtok(NULL, "\n"); // Lit le hash de la dernière compil (sans le \n)
			
			int b = strcmp(h, currentHash); // Le booléen de s'il y a eu un changement
			if(b == 0){ // Le hash a été changé : on le met à jour dans .hash pour la prochinae compilation
				updateHash(currentHash, f);
			}
			fclose(f); // On ferme les ressources
			return b; // On renvoie la comparaison
		}
	}

	// On arrive ici : on n'a jamais trouvé dans le ficher : on ajoute à la fin 
	addHash(nom, currentHash, f);

	fclose(f);

	return 1; // On a jamais trouvé => nouveau fichier (ou dans le doute on recompil)
}

// On modifie le fichier .hash avec la nouvelle valeur de hash
// Hypothèse : le curseur de lecture est à la fin de la ligne correspondante
void updateHash(unsigned char* hash, FILE* f){ // Modifie le nouveau hash
	fseek(f, -SHA256_BLOCK_SIZE - 1, SEEK_CUR); // On recule de SHA256_BLOCK_SIZE pour la taille du hash et -1 supplémentaire pour le \n
	fputs(hash, f); // On écrit le hash (par dessus)
	fseek(f, 1, SEEK_CUR); // On redécale de 1 à l'emplacement initial
}

void addHash(char* nom, unsigned char* hash, FILE* f){
	fseek(f, 0, SEEK_END); // On se rend à la fin du fichier
	fputs("\n", f); // On rajoute un retour à la ligne
	fputs(nom, f);
	fputs(" ", f);
	fputs(hash, f);
}