# CS-265-DCT-Project
Attack on Double Column Transposition with Crib

## Instructions
To compile, run

```
$ make clean; make program
```

To run the program, run the following and follow the instructions on the screen

```
$ ./dcttool.out
```

### Running the Challenge Problem
A file, `./sample/challenge.crib`, is provided to attempt the challenge problem. It is also provided in the makefile as

```
$ make run
```

Make sure the program is already compiled before running this line.

## Notes
This program has been run successfully on macOS. On Linux, the file `decrypt_with_crib.h` at lines 210-211, which frees dynamically allocated memory, causes segmentation faults and core dumps. A temporary fix is to comment out these lines on Linux.
