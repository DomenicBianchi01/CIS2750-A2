all: programs

programs: messages
	gcc -g -Wall -ansi main.c parseFile.c converter.c -o converter
	./converter post.cc
	gcc -c stream.c -o stream.o
	ar cr libstream.a stream.o
	gcc -g -Wall -ansi post.c parseFile.c converter.c -o post -lstream -L.
	gcc -g -Wall -ansi addauthor.c -o addauthor -lstream -L.

messages:
	mkdir messages
	