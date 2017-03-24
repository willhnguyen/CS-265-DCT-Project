# Simple makefile to compile and test the program out

program:
		gcc main.c -o main.out

run:
		./main.out <sample/challenge.crib

clean:
		rm -rf *.out
