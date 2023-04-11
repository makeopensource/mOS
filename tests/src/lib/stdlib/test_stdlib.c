#include "../../test_helper.h"
#include "stdlib/stdlib.h"
#include "stdlib/string.h"


int test_atoi() {
    char *str;
    int exp;

    // tests basic input
    str = "12345";
    exp = 12345;
    ASSERT (atoi(str) != exp);

    // tests discarding spaces
    str = "   42";
    exp = 42;
    ASSERT (atoi(str) != exp);

    // tests leading "+"
    str = "+42";
    exp = 42;
    ASSERT (atoi(str) != exp);

    // tests leading "-"
    str = "-42";
    exp = -42;
    ASSERT (atoi(str) != exp);

    // tests leading 0s are discarded
    str = "0042";
    exp = 42;
    ASSERT (atoi(str) != exp);

    // tests only valid characters are counted
    str = "11x42";
    exp = 11;
    ASSERT (atoi(str) != exp);

    // tests invalid str input
    str = "a1234";
    exp = 0;
    ASSERT (atoi(str) != exp);

    // tests empty input
    str = "";
    exp = 0;
    ASSERT (atoi(str) != exp);

    // tests NULL input
    str = NULL;
    exp = 0;
    ASSERT (atoi(str) != exp);

    char done[] = "test_atoi done\n";
    serialWrite(COM1,  (uint8_t*)(done), sizeof(done) - 1);

    return 0;
}

int test_itoa() {
    int input;
    char *exp;
    int bufsz = 10;
    char buf[bufsz];

    input = 1523;
    exp = "1523";
    itoa(input, buf);
    ASSERT (strncmp(exp, buf, bufsz) != 0);

    input = -1523;
    exp = "-1523";
    itoa(input, buf);
    ASSERT (strncmp(exp, buf, bufsz) != 0);

    char done[] = "test_itoa done\n";
    serialWrite(COM1,  (uint8_t*)(done), sizeof(done) - 1);

    return 0;
}

void test_main() {

    test_atoi();
    test_itoa();

    char done[] = "test_stdlib done\n";
    serialWrite(COM1,  (uint8_t*)(done), sizeof(done) - 1);
}
