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

listeCommandes_t* auxRevList(listeCommandes_t* list1, listeCommandes_t* list2);
listeCommandes_t* revList(listeCommandes_t* list);

void execCommandes(listeCommandes_t* list); // Execute chaque commandes dans la liste

#endif