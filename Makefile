CFLAGS = -Wall -Wextra
CC = gcc
OBJS = main.o

lsv: main.o
	$(CC) $(CFLAGS) main.o -o lsv

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *o lsv
