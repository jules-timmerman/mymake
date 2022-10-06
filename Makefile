regle.o : regle.c regle.h 
	gcc -c regle.c

listeRegle.o : listeRegle.c listeRegle.h regle.h
	gcc -c listeRegle.o