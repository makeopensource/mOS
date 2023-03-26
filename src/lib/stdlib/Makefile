CC := gcc
CFLAGS := -g -Wall -Werror

# To add a test, place the name of the executable file 
# you want to run here:
TESTS := test_stdlib test_string test_stdio

# The name of the testing directory
TEST_DIR := tests

# To add a new standard library file, add the 
# .c file here:
STD_LIB := string.c stdlib.c stdio.c

TEST_OBJS := $(foreach test, $(TESTS), $(test).o)
DEPENDENCIES := $(foreach std_file, $(STD_LIB), $(std_file:.c=.o))


all: $(TEST_OBJS) $(DEPENDENCIES) 
	for test in $(TESTS); do $(CC) -o $$test $$test.o $(DEPENDENCIES); done

test: all
	for test in $(TESTS); do ./$$test; done

%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TESTS) *.o

.PHONY:
	test all clean
