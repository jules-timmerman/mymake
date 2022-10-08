#ifndef LISTECOMMANDES_H
#define LISTECOMMANDES_H

typedef struct listeCommandes listeCommandes_t;

// Une liste de commandes est soit le pointeur NULL,
// soit un couple composé d'une commande et d'un pointeur vers une liste de commandes
struct listeCommandes{
	char* commande;
	listeCommandes_t* next; // NULL quand le dernier
};

listeCommandes_t* createListeCommands(void);
void freeListeCommands(listeCommandes_t* c);

listeCommandes_t* addCommande(listeCommandes_t* list, char* c);

void execCommandes(listeCommandes_t* list); // Execute chaque commandes dans la liste

#endif