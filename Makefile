main: main.o regle.o listeRegles.o hash.o listeCommandes.o
	gcc -o mymake main.o regle.o listeRegles.o hash.o listeCommandes.o && cp mymake testproj/

main.o: main.c regle.h listeRegles.h
	gcc -c main.c

regle.o : regle.c regle.h 
	gcc -c regle.c

listeRegles.o : listeRegles.c listeRegles.h regle.h
	gcc -c listeRegles.c

hash.o : hash.c hash.h
	gcc -c hash.c

listeCommandes.o : listeCommandes.c listeCommandes.h
	gcc -c listeCommandes.c
