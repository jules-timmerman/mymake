main: main.o regle.o listeRegles.o sha256.o listeCommandes.o
	gcc -g -o mymake main.o regle.o listeRegles.o sha256.o listeCommandes.o && cp mymake testproj/

main.o: main.c regle.h listeRegles.h
	gcc -c -g main.c

regle.o: regle.c regle.h sha256.h
	gcc -c -g regle.c

listeRegles.o: listeRegles.c listeRegles.h regle.h
	gcc -c -g listeRegles.c

listeCommandes.o: listeCommandes.c listeCommandes.h
	gcc -c -g listeCommandes.c

sha256.o: sha256.c sha256.h
	gcc -c -g sha256.c
