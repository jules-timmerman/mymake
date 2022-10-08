#include <string.h>
#include <stdio.h>

//main: main.o regle.o listeRegles.o
//	gcc -o main main.o regle.o listeRegles.o


//regle_t* createRegle(char* nom, char** prerequis, int lenPrerequis, char** commandes, int lenCommandes){
//	regle_t* r = malloc(sizeof(regle_t));
//	r->nom = nom;
//	r->prerequis = prerequis;
//	r->commandes = commandes;
//	r->lastModified = getLastModified(nom);	
//	return r;
//}

int main () {
   char str[80] = "main: main.o regle.o listeRegles.o";
   const char s[2] = " ";
   char *token;
   
   /* get the first token */
   token = strtok(str, ":");
   
   /* walk through other tokens */
   while( token != NULL ) {
      printf( " %s\n", token );
    
      token = strtok(NULL, " ");
   }
   
   return(0);
}