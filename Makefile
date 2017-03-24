# Simple makefile to compile and test the program out

program:
		gcc main.c -o dcttool.out

run:
		./dcttool.out <sample/challenge.crib

clean:
		rm -rf *.out
