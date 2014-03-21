CC=gcc
CFLAGS=-c -Wall -O2 -std=c99 -MD -g --coverage
LDFLAGS=--coverage -g
SOURCES=test.c avl_tree.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=out
include $(wildcard *.d)

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o out *.d *.gcno *.gcda *.gcov *~
