main: main.o a.o b.o c.o d.o
	gcc -o main main.o a.o b.o c.o d.o

main.o: main.c c.h d.h
	gcc -c main.c

a.o: a.h a.c
	gcc -c a.c

b.o: b.h b.c
	gcc -c b.c

c.o: c.c c.h a.h b.h
	gcc -c c.c

d.o: d.h d.c a.h
	gcc -c d.c