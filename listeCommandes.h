#ifndef LISTECOMMANDES_H
#define LISTECOMMANDES_H

typedef struct listeCommandes listeCommandes_t;

// NULL ou couple (commande, pointeur vers liste de commandes)
struct listeCommandes{
	char* commande;
	listeCommandes_t* next; // NULL quand le dernier
};

// Crée une liste vide de commandes (= le pointeur NULL)
listeCommandes_t* createListeCommands(void);

// Libere une liste de commandes (sans libérer les commandes elles-mêmes)
void freeListeCommands(listeCommandes_t* c, int freeContent);

// Ajoute une commande [r] a la liste de commandes [list] (en tête)
listeCommandes_t* addCommande(listeCommandes_t* list, char* c);

// Renvoie un pointeur vers [list1] renversée dans la liste [list2]
listeCommandes_t* auxRevList(listeCommandes_t* list1, listeCommandes_t* list2);

// Renvoie un pointeur vers la liste renversée de [list]
listeCommandes_t* revListCommande(listeCommandes_t* list);

// Execute chaque commande de la liste
void execCommandes(listeCommandes_t* list);

#endif