CC = gcc
CFLAGS= -std=c99
LIB =

all: server client

server: server.c
	$(CC) -o $@ $< ${CFLAGS} ${LIB}

client: client.c
	$(CC) -o $@ $< ${CFLAGS} ${LIB}

clean:
	rm server client
