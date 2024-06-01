# Compiler and flags
CC=gcc
CFLAGS=-I.

# Build target
client: client.o 
	$(CC) -o client client.o

# Object files
client.o: client.c
	$(CC) -c client.c $(CFLAGS)

# Clean target
clean:
	rm -f *.o client

