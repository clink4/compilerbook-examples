bminor: bin/main.o bin/scanner.o
	gcc bin/main.o bin/scanner.o -o bminor

bin/main.o: main.c token.h
	gcc main.c -c -o bin/main.o

bin/scanner.o: scanner.c token.h
	gcc scanner.c -c -o bin/scanner.o

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

clean:
	rm -f scanner.c bin/scanner.o bin/main.o bminor
