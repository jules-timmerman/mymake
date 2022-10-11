#ifndef LISTECOMMANDES_H
#define LISTECOMMANDES_H

typedef struct listeCommandes listeCommandes;

// NULL ou couple (commande, pointeur vers liste de commandes)
struct listeCommandes{
	char* commande;
	listeCommandes* next; // NULL quand le dernier
};

// Crée une liste vide de commandes (= le pointeur NULL)
listeCommandes* createListeCommands(void);

// Libere une liste de commandes (sans libérer les commandes elles-mêmes)
void freeListeCommands(listeCommandes* c, int freeContent);

// Ajoute une commande [r] a la liste de commandes [list] (en tête)
listeCommandes* addCommande(listeCommandes* list, char* c);

// Renvoie un pointeur vers [list1] renversée dans la liste [list2]
listeCommandes* auxRevList(listeCommandes* list1, listeCommandes* list2);

// Renvoie un pointeur vers la liste renversée de [list]
listeCommandes* revListCommande(listeCommandes* list);

// Execute chaque commande de la liste
void execCommandes(listeCommandes* list);

#endif