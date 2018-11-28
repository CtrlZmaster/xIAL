# Project: IAL ...
# File: Makefile
# Title:
# Description: -
# Author: Michal Pospíšil (xpospi95@stud.fit.vutbr.cz)

##### VARIABLES
CFLAGS = -Wall -Wextra -pedantic #-Werror
EXTRAFLAGS = -g

##### TARGETS - first target is default
sptree: main.o loadfile.o
	gcc main.o loadfile.o $(CFLAGS) $(EXTRAFLAGS) -o sptree

clean:
	rm -f *.o

pack:
	zip xpospi95.zip *.c *.h dokumentacia.pdf Makefile

##### OBJECT FILES
main.o: main.c
	gcc -c main.c $(CFLAGS) $(EXTRAFLAGS) -o main.o

loadfile.o: loadfile.c loadfile.h
	gcc -c loadfile.c $(CFLAGS) $(EXTRAFLAGS) -o loadfile.o