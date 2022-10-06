#include <stdlib.h>
#include <stdio.h>

#include "regle.h"
#include "listeRegles.h"

listeRegles_t readMakefile(FILE* makefile){

    if (makefile == NULL){
        return createListeRegle();
    }
    else{
        char ligne;
        while (getline(makefile, "%c", &ligne) != EOF)
        {
            
        }
        

    }
}


void main(){
    printf("Hello, world.");
}