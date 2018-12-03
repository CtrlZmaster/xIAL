# Project: IAL ...
# File: Makefile
# Title: 7. Minimum weight spanning tree
# Description: -
# Author: Michal Pospíšil (xpospi95@stud.fit.vutbr.cz)

##### VARIABLES
CFLAGS = -Wall -Wextra -pedantic #-Werror
EXTRAFLAGS = -g

##### TARGETS - first target is default
sptree: main.o loadfile.o
	gcc main.o loadfile.o $(CFLAGS) $(EXTRAFLAGS) -o sptree

sptree-timer: main-timer.o loadfile.o
	gcc main-timer.o loadfile.o $(CFLAGS) $(EXTRAFLAGS) -o sptree-timer

ldfl-test: loadfile-test.o loadfile.o
	gcc loadfile-test.o loadfile.o $(CFLAGS) $(EXTRAFLAGS) -o ldfl-test

clean:
	rm -f *.o

pack:
	zip -r ial-nahr-proj-7-18Z.zip *.c *.h examples dokumentacia.pdf Makefile

##### OBJECT FILES
main.o: main.c main.h
	gcc -c main.c $(CFLAGS) $(EXTRAFLAGS) -o main.o

main-timer.o: main.c main.h
	gcc -c main.c -DTIME $(CFLAGS) $(EXTRAFLAGS) -o main-timer.o

loadfile.o: loadfile.c loadfile.h
	gcc -c loadfile.c $(CFLAGS) $(EXTRAFLAGS) -o loadfile.o

loadfile-test.o: loadfile-test.c loadfile.c loadfile.h
	gcc -c loadfile-test.c $(CFLAGS) $(EXTRAFLAGS) -o loadfile-test.o