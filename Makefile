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

ldfl-test: loadfile-test.o loadfile.o
	gcc loadfile-test.o loadfile.o $(CFLAGS) $(EXTRAFLAGS) -o ldfl-test

clean:
	rm -f *.o

pack:
	zip xpospi95.zip *.c *.h dokumentacia.pdf Makefile

##### OBJECT FILES
main.o: main.c main.h
	gcc -c main.c $(CFLAGS) $(EXTRAFLAGS) -o main.o

loadfile.o: loadfile.c loadfile.h
	gcc -c loadfile.c $(CFLAGS) $(EXTRAFLAGS) -o loadfile.o

loadfile-test.o: loadfile-test.c loadfile.c loadfile.h
	gcc -c loadfile-test.c $(CFLAGS) $(EXTRAFLAGS) -o loadfile-test.o