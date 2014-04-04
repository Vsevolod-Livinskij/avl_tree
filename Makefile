CC=gcc
CFLAGS=-c -Wall -std=c99 -g --coverage
LDFLAGS=--coverage -g
SOURCES=test.c avl_tree.c
HEADERS=avl_tree_lib.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=out

all: $(SOURCES) $(EXECUTABLE) $(HEADERS)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o out *.gcno *.gcda *.gcov *~
