# The mOS C Standard Library

The mOS standard library is a library of C functions modelled off C11 
standard library equivilents.

## Functions

| Name		| File		| Usage	    |
|---------------|---------------|-----------------------------------------------|
| snprintf	| stdio.h 	| Print formatted strings to output buffer	|
| vsnprintf | stdio.h   | Print formatted strings to output buffer (inputs va_list)
| atoi		| stdlib.h	| Convert strings to integers			|
| \*itoa\_s	| stdlib.h	| Convert integers to strings			|
| \*strnlen\_s	| string.h	| Find length of a string			|
| \*strcpy\_s	| string.h	| Copy a string from one pointer to another	|
| strncmp	| string.h	| Compare the contents of two strings		|
| memcpy	| string.h	| Copy bytes from one buffer to another		|
| memset    | string.h  | Sets n bytes to character c               |

\* Note that the "_s" stands for "_safe," many of these functions have security
vulnerabilities that can allow them to overwrite buffers. These functions include
an additional parameter to restrict the maximum read and write to the size of the
buffer.

## Tests

Tests are written in the `tests/` directory. For directions on testing, consult the [README](../../../tests/README.md) in the tests directory.

*See Makefile for more details.*
