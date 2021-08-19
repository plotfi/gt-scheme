CC=gcc
CFLAGS=-W -Wall -O2 -ansi -pedantic
EXE=gtscheme
SPLINT = ~cs2130/pub/bin/2130splint

$(EXE): semant.o ast.h main.c lexer.o parser.o
	$(CC) $(CFLAGS) -o $(EXE) semant.o lexer.o parser.o main.c

splint: lexer.c parser.c main.c
	$(SPLINT) *.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

semant.o: semant.c semant.h ast.h
	$(CC) $(CFLAGS) -c semant.c

wax:
	rm -f core.*

clean:
	rm -f *~ \#*\# $(EXE) lexer.o parser.o semant.o
