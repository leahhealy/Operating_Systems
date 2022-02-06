all: filecopy treecopy

filecopy: filecopy.o
	gcc filecopy.o -o filecopy

treecopy: treecopy.o
	gcc treecopy.o -o treecopy

filecopy.o: filecopy.c
	gcc -c -g --std=c99 -Wall filecopy.c -o filecopy.o

treecopy.o: treecopy.c
	gcc -c -g --std=c99 -Wall treecopy.c -o treecopy.o

clean:
	rm -f filecopy treecopy *.o
