# Testing

Testing is an integral part of making any large scale project.  
As such we have created an automated testing environment!  

## Building

Building can be done in two ways. Indirectly through `make test`,
or directly through `make` in the `tests` directory.  
  
`make clean` in either Makefile will clean

## Running

Running can be done in two ways. Indirectly through `make test`,  
or directly through running `test.py` (don't forget to build).

## Creating Tests

Creating tests requires creating at least two new files.  
A `.c` file anywhere in `tests/src` and a `.expect` of the same name (without the .c) anwhere in the `tests/expected`.  
The `.c` file can include any OS header and `tests/src/test_helper.h` is recommended. `ASSERT(condition)` in the helper will produce no output on success but will write to serial on failure. The `.c` must have some serial output to compare against the expected.  
The `.expect` file consists of all the output the `.c` file will produce when everything works as expected. Output is always in the form of serial output.

### Dependencies

Testing requires the same dependencies as normal operation along with the following:

* Python 3.8
