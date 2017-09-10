TARGET  = list
CC      = gcc
FLAGS   = -g -std=c11 -Wall -Wextra -pedantic -fPIC -DDEBUG=0
OBJECTS = $(wildcard *.c)

.PHONY: default all clean

default: $(TARGET)
all: default

$(TARGET):
	$(CC) $(FLAGS) $(OBJECTS) -o $(TARGET)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
