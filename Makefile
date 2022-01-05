CFLAGS = -Wall -Wextra
CC = gcc
OBJS = main.o

lsv: main.o
	$(CC) $(CFLAGS) main.o -o lsv

install: lsv
	cp lsv ~/.local/bin

main.o: main.c exts.h colors.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *o lsv
