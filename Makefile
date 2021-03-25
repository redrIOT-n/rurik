all: main

main: main.c
	gcc -g -lncurses main.c -o main

clean:
	rm -f main
