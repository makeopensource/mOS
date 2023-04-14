#include "../../test_helper.h"
#include "stdlib/stdlib.h"
#include "stdlib/string.h"


int test_atoi(char *in, int exp) {
    ASSERT (atoi(in) == exp);
    return -1;
}

// expsz is the last argument because it is the size of the second argument
int test_itoa(int in, char *exp, int expsz) {
    int bufsz = 100;
    char buf[bufsz];
    itoa(in, buf);

    ASSERT (strncmp(exp, buf, expsz) == 0);
    return 0;
}

void test_main() {

    test_atoi("12345", 12345);
    test_atoi("    42", 42);
    test_atoi(" +42", 42);
    test_atoi(" -42", -42);
    test_atoi(" 0042", 42);
    test_atoi(" 11x42", 11);
    test_atoi(" x1234", 0);
    test_atoi(" ab3", 0);

    test_itoa(1523, "1523", 4);
    test_itoa(-1523, "-1523", 5);
    test_itoa(0, "0", 1);
    test_itoa(-0, "0", 1);

    char done[] = "test_stdlib done\n";
    serialWrite(COM1,  (uint8_t*)(done), sizeof(done) - 1);
}
