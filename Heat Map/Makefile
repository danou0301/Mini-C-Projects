CC= gcc
CFLAGS= -Wextra -Wall -Wvla -std=c99

ex3: calculator.o reader.o heat_eqn.o
	$(CC) calculator.o reader.o heat_eqn.o -o ex3

all: ex3
	ex3 input.txt

calculator.o: calculator.c  calculator.h
	$(CC) $(CFLAGS) -c calculator.c

reader.o: reader.c calculator.h  heat_eqn.h
	$(CC) $(CFLAGS) -c reader.c

heat_eqn.o: heat_eqn.c heat_eqn.h
	$(CC) $(CFLAGS) -c heat_eqn.c

clean:
	rm -f *.o ex3
