CC=gcc
CFLAGS=-I.

main: main.c requests.c helpers.c buffer.c find.c parson.c commands.c
	$(CC) -o main main.c requests.c helpers.c buffer.c find.c parson.c commands.c -Wall

run: main
	./main

clean:
	rm -f *.o main
