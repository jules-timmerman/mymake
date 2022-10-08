#include "hash.h"


struct byteAddition{
	unsigned char sum;
	int carry; // Booléen sur si on a une retenu ou pas (l'overflow)
};



// CF biblio
// Version modifiée : hash les size premiers caractères de str
unsigned long hashStr(unsigned char *str, int size){
        unsigned long hash = 5381;
        int c;

		for(int i = 0; i < size; i++){
            hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + str[i] */
		}

        return hash;
}

// On va hash le fichier entier : pour ca, nous allons lire des bouts de BLOCK_SIZE bytes, les sommer jusqu'à arriver à l'EOF puis ensuite hash cette séquence
unsigned long hashFile(char* name){
	FILE* f = fopen(name,"rb"); // On ouvre avec b pour traiter le fichier avec des bytes

	if(f == NULL){ // Garde fou
		printf("Couldn't open file %s for hash", name);
		exit(1); // Crash le programme
	}
	
	unsigned char buffer[BLOCK_SIZE]; // Notre somme résultante, initialisée à 0
	for(int i = 0; i < BLOCK_SIZE; i++){
		buffer[i] = 0;
	}

	unsigned char nextBlock[BLOCK_SIZE]; // Le bloc suivant

	byteAddition_t add; // Notre struct de résultat d'addition (que l'on mettra à jour au fur et à mesure)
	add.sum = 0;
	add.carry = 0;


	long fileSize = getFileSize(f); // On récupère la taille du fichier

	for(int i = 0; i < fileSize/BLOCK_SIZE; i++){ // On lit des blocs complets 
		fread(nextBlock, BLOCK_SIZE, 1, f); // On lit le bloc suivant
		sumBlocks(&add, buffer, nextBlock); // On somme les blocs
	}

	// Il reste potentiellement un dernier bloc qui n'est pas de taille suffisante : on va rajouter des 0 devant (aka à la fin du fichier)
	if(fileSize % BLOCK_SIZE != 0){ // Dans le cas inégal
		long bytesLeft = fileSize - ftell(f); // Le nombre de bytes restantes
		for(int i = 0; i < BLOCK_SIZE - bytesLeft; i++){
			nextBlock[i] = 0; // On rajoute les 0 en vidant (cf nextBlock = 0000xxxx)
		}
		fread(nextBlock+(BLOCK_SIZE - bytesLeft), bytesLeft, 1, f); // On rempli à partir de l'indice BLOCK_SIZE - bytesLeft (oui! arithmétique des pointeurs)
		sumBlocks(&add, buffer, nextBlock); // On ajoute le dernier bloc
	}

	unsigned long h = hashStr(buffer, BLOCK_SIZE);

	fclose(f);

	return h;
}

void sumBytes(byteAddition_t* add, unsigned char b1, unsigned char b2){
	unsigned char b3 = b1 + b2;
	add->sum = b3;
	if(b3 < b1){ // On a une retenu
		add->carry = 1;
	}else{ // Pas de retenu
		add->carry = 0;
	}
}

long getFileSize(FILE* f){
	long currentIndex = ftell(f); // On se souvient de la position initiale
	fseek(f, 0, SEEK_END); // On va à la fin
	long ret = ftell(f); // On note la taille
	fseek(f, currentIndex, SEEK_SET); // On retourne à la position initiale
	return ret;
}

// Calcule la somme des blocs
// Utilise add pour garder les retenus d'une exec à l'autre
void sumBlocks(byteAddition_t* add, char buffer[BLOCK_SIZE], unsigned char nextBlock[BLOCK_SIZE]){
	for(int j = BLOCK_SIZE - 1; j > -1; j--){ // On somme les bytes 1 à 1 (en partant de la fin)
			if(add->carry == 1){
				sumBytes(add, nextBlock[j], buffer[j]);
				buffer[j] = add->sum + 1; // On rajoute la retenu (on aura pas de nouveau d'overflow a priori (cf 255+255 = 254)
			}else{
 				sumBytes(&add, nextBlock[j], buffer[j]);
				buffer[j] = add->sum;
			}
		}

}
