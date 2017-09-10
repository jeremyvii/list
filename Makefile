FLAGS:= -g -std=c11 -Wall -Wextra -pedantic -fPIC -DDEBUG=0

default: list

list: 
	gcc $(FLAGS) main.c
