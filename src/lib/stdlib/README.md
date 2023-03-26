# The mOS C Standard Library

The mOS standard library is a library of C functions modelled off their C 
standard library equivilents in C99.

## Functions

| Name		| File		| Usage						|
|---------------|---------------|-----------------------------------------------|
| \*sn\_printf	| stdio.h 	| Print formatted strings to output buffer	|
| atoi		| stdlib.h	| Convert strings to integers			|
| \*strnlen\_s	| string.h	| Find length of a string			|
| \*strcpy\_s	| string.h	| Copy a string from one pointer to another	|
| strncmp	| string.h	| Compare the contents of two strings		|
| memcpy	| string.h	| Copy bytes from one buffer to another		|

\* Note that the "_s" stands for "_safe," many of these functions have security
vulnerabilities that can allow them to overwrite buffers. These functions include
an additional parameter to restrict the maximum read and write to the size of the
buffer.

Also, note that `sn_printf` is a deviation from the name `snprintf` in C99, this was
done so that tests could use the standard implementation of printf.

## Tests

Tests are written in the `tests/` directory. Each test is simply a C file with a
main function. To add a test to an existing test file, consult the file for 
more details.

To run all tests, use the command `$ make test`.

To add a new test file, first add the name of the test file to the TESTS variable.

To add a new stdlib file, add the `.c` file to STD\_LIB.

To remove all object files and executables, run `$ make clean`.

*See Makefile for more details.*
