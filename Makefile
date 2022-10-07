main: main.o regle.o listeRegles.o
	gcc -o main main.o regle.o listeRegles.o

main.o: main.c regle.h listeRegles.h
	gcc -c main.c

regle.o : regle.c regle.h 
	gcc -c regle.c

listeRegles.o : listeRegles.c listeRegles.h regle.h
	gcc -c listeRegle.c