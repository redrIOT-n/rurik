CC=gcc
CFLAGS=-g -Wall
BIN=bin/rurik
OBJS=obj/attr.o obj/xmlparse.o obj/editor.o obj/file_handle.o obj/file_browser.o obj/menubar.o obj/terminal.o obj/main.o
SUBMITNAME=rurik.zip

all:$(BIN)

bin/rurik: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lncurses -lcdk -lm -o $(BIN)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/* bin/*

git:
	git add .
	git commit -m "blablabla"
	git push origin main
