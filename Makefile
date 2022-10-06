main: main.o regle.o listeRegles.o
	gcc -o main main.o regle.o listeRegles.o

regle.o : regle.c regle.h 
	gcc -c regle.c

listeRegle.o : listeRegle.c listeRegle.h regle.h
	gcc -c listeRegle.o