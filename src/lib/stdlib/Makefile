CC := gcc
CFLAGS := -g -Wall -Werror

TESTS := test
TEST_OBJS := $(foreach test, $(TESTS), $(test).o)
DEPENDENCIES := string.o stdlib.o # stdio.o

all: $(TEST_OBJS) $(DEPENDENCIES)
	for test in $(TESTS); do $(CC) -o $$test $$test.o $(DEPENDENCIES); done

test: all
	for test in $(TESTS); do ./$$test; done

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TESTS) *.o

.PHONY:
	test all clean
