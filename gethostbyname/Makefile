CC = gcc
all: getHostByName getHostByAddr getHostName

getHostByName: getHostByName.c
	$(CC) -o $@ $<

getHostByAddr: getHostByAddr.c
	$(CC) -o $@ $<

getHostName: getHostName.c
	$(CC) -o $@ $<

getPeerName: getPeerName.c
	$(CC) -o $@ $<

clean:
	rm getHostByName getHostByAddr getHostName
