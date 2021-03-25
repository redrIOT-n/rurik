all: main

main: main.c
	gcc -g -lncurses main.c -lcdk -o main

clean:
	rm -f main
