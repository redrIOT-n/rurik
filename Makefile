all: main

main: main.c
	gcc -g main.c -lncurses -lcdk -o main

clean:
	rm -f main
