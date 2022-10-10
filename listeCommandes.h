#ifndef LISTECOMMANDES_H
#define LISTECOMMANDES_H

typedef struct listeCommandes listeCommandes_t;

// Une liste de commandes est soit le pointeur NULL,
// soit un couple composé d'une commande et d'un pointeur vers une liste de commandes
struct listeCommandes{
	char* commande;
	listeCommandes_t* next; // NULL quand le dernier
};

// Cree une liste vide de commandes (= le pointeur NULL)
listeCommandes_t* createListeCommands(void);

// Libere une liste de commandes (sans libérer les commandes elles-mêmes)
void freeListeCommands(listeCommandes_t* c);

// Ajoute une commande [r] a la liste de commandes [list] (en tête)
listeCommandes_t* addCommande(listeCommandes_t* list, char* c);

// Renvoie un pointeur vers [list1] renversée dans la liste [list2]
listeCommandes_t* auxRevList(listeCommandes_t* list1, listeCommandes_t* list2);

// Renvoie un pointeur vers la liste renversée de [list]
listeCommandes_t* revList(listeCommandes_t* list);

// Execute chaque commande de la liste
void execCommandes(listeCommandes_t* list);

#endif