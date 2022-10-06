#include <stdlib.h>
#include <stdio.h>

#include "regle.h"
#include "listeRegles.h"


/* Transforme une regle de type main: main.o regle.o listeRegles.o */
listeRegles_t readMakefile(FILE* makefile){
    char ligne[512];
    listeRegles_t* liste = createListeRegle(); 
    while (getline(makefile, "%s", &ligne)){
        if (ligne[0] != '	') {
            char** decoupe;
            regle_t* nouvelleRegle;
            decoupe = strtok (ligne, ' :\t');
            nouvelleRegle = createRegle(decoupe[0], decoupe, decoupe);
            liste = addRegle(liste, nouvelleRegle);
           }
    }
    return *liste;
}


void main(){
    printf("Hello, world.");
}