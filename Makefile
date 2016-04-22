default: release

release:
	gcc -lm -ansi -Wall -o proj proj.c

main: list.o main.o
	gcc -o main list.o main.o
list.o: list.c list.h
	gcc -g -c list.c
main.o: main.c list.h
	gcc -g -c main.c

run: main
	./main

clean:
	rm *.o