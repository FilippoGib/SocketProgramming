# Compiler and flags
CC=gcc
CFLAGS=-I.

# Build target
server: server.o
	$(CC) -o server server.o

# Object files
server.o: server.c
	$(CC) -c server.c $(CFLAGS)

# Clean target
clean:
	rm -f *.o server

