main: main.o regle.o listeRegles.o hash.o listeCommandes.o
	gcc -g -o mymake main.o regle.o listeRegles.o hash.o listeCommandes.o && cp mymake testproj/

main.o: main.c regle.h listeRegles.h
	gcc -c -g main.c

regle.o: regle.c regle.h hash.h
	gcc -c -g regle.c

listeRegles.o: listeRegles.c listeRegles.h regle.h
	gcc -c -g listeRegles.c

hash.o: hash.c hash.h
	gcc -c -g hash.c

listeCommandes.o: listeCommandes.c listeCommandes.h
	gcc -c -g listeCommandes.c
