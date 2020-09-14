CC= gcc
FLAG= -c -Wall -Wextra -Wvla -std=c99
FILES= ex2.tar battleships.c battleships.h battleships_game.c Makefile


all: battleships_game.o battleships.o
	$(CC) battleships_game.o battleships.o -o ex2

battleships_game: battleships_game.c battleships.h
	$(CC) $(FLAG) battleships_game.c -o battleships.o

battleships : battleships.c battleships.h
	$(CC) $(FLAG) battleships.c -o battleships.o


tar:
	tar -cf $(FILES)

clean:
	rm -f *.o ex2