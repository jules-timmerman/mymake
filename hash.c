#include "hash.h"

#include "sha256.h"

#define BLOCK_SIZE 64



void hashFile(char* nom, unsigned char* hash){
	SHA256_CTX* ctx = malloc(sizeof(SHA256_CTX));
	sha256_init(ctx); // On initialise notre hash

	FILE* f = fopen(nom, "rb"); // On lit en mode binaire ici
	if(f == NULL){
		printf("Couldn't open file %s for hash\n", nom);
		exit();
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
