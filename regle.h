#ifndef REGLE_H
#define REGLE_H

typedef struct regle regle_t; // Création de la regle

regle_t* createRegle(char* nom, char** prerequis, char** commandes);
void freeRegle(regle_t* r);

#endif REGLE_H