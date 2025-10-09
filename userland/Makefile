CC=gcc
CFLAGS=-O2 -Wall

all: pku-check pku-demo

pku-check: pku-check.c
	$(CC) $(CFLAGS) -o pku-check pku-check.c

pku-demo: pku-demo.c
	$(CC) $(CFLAGS) -o pku-demo pku-demo.c

clean:
	rm -f pku-check pku-demo

