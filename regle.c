#include "regle.h"

#include "sha256.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 64


// VERSION SANS HASH AVEC DATES
// regle* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes* commandes){
// 	regle* r = malloc(sizeof(regle));
// 	r->nom = nom;
// 	r->prerequis = prerequis;
// 	r->lenPrerequis = lenPrerequis;
// 	r->commandes = commandes;
// 	r->lastModified = getLastModified(nom);	
// 	return r;
// }

regle* createRegle(char* nom, char** prerequis, int lenPrerequis, listeCommandes* commandes){
	regle* r = malloc(sizeof(regle));
	r->nom = nom;
	r->prerequis = prerequis;
	r->lenPrerequis = lenPrerequis;
	r->commandes = commandes;
	r->hashModified = hashWasModified(nom); // 1 si on a eu une modification, 0 sinon
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
// 1 : modification du fichier
// Format de .hash : nom hash
int hashWasModified(char* nom){
	// On regarde si le fichier .hash existe (sinon on le crée)
	if(access(".hash", F_OK) != 0){  // N'existe pas : on le crée
		FILE* f = fopen(".hash", "w");
		if(f == NULL){
			printf("Couldn't create .hash\n");
			exit(1);
		}
		fclose(f);
	}
	FILE* f = fopen(".hash", "r+"); // r+ lecture et écriture depuis le début du fichier sans écraser tout.
	if(f == NULL){
		printf("Couldn't open .hash (no file or problem ?)\n");
		return 1; // Renvoie 1 : dans le doute, on va recompiler 
	}

	unsigned char currentHash[SHA256_BLOCK_SIZE];
	hashFile(nom, currentHash); // Calcule le hash actuel

	char prettyHash[2*SHA256_BLOCK_SIZE + 1]; // x2 car chaque byte va prendre 2 places
	prettyHash[2*SHA256_BLOCK_SIZE] = '\0'; // On rajoute le \0 pour pouvoir manipuler avec strcmp. x2 car un byte est écrit sur 2 caractères hexadécimal (00 -> FF)
	for(int i = 0; i < SHA256_BLOCK_SIZE; i++){
		sprintf(prettyHash+(2*i), "%02x", currentHash[i]); // 02 : on rajoute les 0 pour être sûr que chaque byte prends 2 places / caractères
	}

	char* line = NULL;
	size_t n = 0;

	while(getline(&line, &n, f) != -1){ // On lit jusqu'à l'EOF dans le pire des cas à la recherche de notre nom (le fichier)
		char* s = strtok(line, " ");
		if(strcmp(s, nom) == 0){ // La règle correspondante (on a trouvé)
			char* h = strtok(NULL, "\n"); // Lit le hash de la dernière compil (sans le \n)
			
			int b = strcmp(h, prettyHash); // Le booléen de s'il y a eu un changement (b == 0 ssi h = prettyHash ssi aucun changement)
			if(b != 0){ // Le hash a été changé : on le met à jour dans .hash pour la prochinae compilation
				updateHash(prettyHash, f);
			}
			fclose(f); // On ferme les ressources
			return b != 0; // On renvoie la comparaison (b == 1 = 1 ssi b == 1 ssi changement) 
		}
	}

	// On arrive ici : on n'a jamais trouvé dans le ficher : on ajoute à la fin 
	addHash(nom, prettyHash, f);

	fclose(f);

	return 1; // On a jamais trouvé => nouveau fichier (ou dans le doute on recompil)
}

// On modifie le fichier .hash avec la nouvelle valeur de hash
// Hypothèse : le curseur de lecture est à la fin de la ligne correspondante
void updateHash(char* prettyHash, FILE* f){ // Modifie le nouveau hash
	fseek(f, -2*SHA256_BLOCK_SIZE - 1, SEEK_CUR); // On recule de 2*SHA256_BLOCK_SIZE pour la taille du prettyHash et -1 supplémentaire pour le \n
	fputs(prettyHash, f);
	fseek(f, 1, SEEK_CUR); // On redécale de 1 à l'emplacement initial
}

void addHash(char* nom, char* prettyHash, FILE* f){
	fseek(f, 0, SEEK_END); // On se rend à la fin du fichier
	fputs(nom, f);
	fputs(" ", f);
	fputs(prettyHash, f);
	fputs("\n", f); // On rajoute un retour à la ligne
}


void hashFile(char* nom, unsigned char* hash){
	SHA256_CTX* ctx = malloc(sizeof(SHA256_CTX));
	sha256_init(ctx); // On initialise notre hash

	FILE* f = fopen(nom, "rb"); // On lit en mode binaire ici
	if(f == NULL){
		printf("Couldn't open file %s for hash\n", nom);
		exit(1);
	}
	
	// On lit le fichier au fur et à mesure par blocs
	unsigned char* data = malloc(sizeof(char) * BLOCK_SIZE);
	size_t bytesRead = 0;
	do{ // Jusqu'à l'EOF
		bytesRead = fread(data, 1, BLOCK_SIZE, f);
		sha256_update(ctx, data, bytesRead); // On met à jour l'état de notre hash avec ce qu'on vient de lire
	}while(bytesRead != 0);

	sha256_final(ctx, hash); // On finalise le calcul de hash et récupè le résultat


	free(data);
	free(ctx);
}