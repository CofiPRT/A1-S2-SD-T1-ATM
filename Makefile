CC = gcc
CFLAGS = -g -Wall -Wextra -std=gnu89
HEADERS = main_header.h
OBJECTS = source.o alloc_free_functions.o support_functions.o admin_functions.o client_functions.o
EXEC = atm

.PHONY = default build clean

default: build

build: $(EXEC)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -O3 -o $@

run: build
	./$(EXEC)

run_valgrind: build
	-valgrind --track-origins=yes --leak-check=full -q --log-file=v.out "./$(EXEC)"

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC)