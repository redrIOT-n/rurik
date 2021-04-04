CC=gcc
CFLAGS=-g -Wall

all:myapp

xmlparse.o: xmlparse.c
	$(CC) $(CFLAGS) -c xmlparse.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

myapp: main.o xmlparse.o
	$(CC) $(CFLAGS) main.o xmlparse.o -o myapp

clean:
	rm -f *.o myapp
