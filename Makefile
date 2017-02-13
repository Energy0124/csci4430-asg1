CPPC = g++
CPPFLAGS= -std=c++11
LIB =

all: server client

server: server.cpp
	$(CPPC) -o $@ $< ${CPPFLAGS} ${LIB}

client: client.cpp
	$(CPPC) -o $@ $< ${CPPFLAGS} ${LIB}

clean:
	rm server client
