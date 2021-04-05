CC=gcc
CFLAGS=-g -Wall

all:myapp

attr.o: attr.c
	$(CC) $(CFLAGS) -c attr.c

xmlparse.o: xmlparse.c
	$(CC) $(CFLAGS) -c xmlparse.c

myapp: attr.o xmlparse.o
	$(CC) $(CFLAGS) attr.o xmlparse.o -o myapp

clean:
	rm -f *.o myapp
