CXXFLAGS:= -g -std=c11 -Wall -Wextra -pedantic -fPIC -DDEBUG=0

default: list

list: 
	gcc $(CXXFLAGS) main.c
