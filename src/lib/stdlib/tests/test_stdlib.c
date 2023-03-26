#include "../stdlib.h"
#include <stdio.h>


int test_atoi() {
    char *str;
    int exp;

    // tests basic input
    str = "12345";
    exp = 12345;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests discarding spaces
    str = "   42";
    exp = 42;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests leading "+"
    str = "+42";
    exp = 42;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests leading "-"
    str = "-42";
    exp = -42;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests leading 0s are discarded
    str = "0042";
    exp = 42;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests only valid characters are counted
    str = "11x42";
    exp = 11;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests invalid str input
    str = "a1234";
    exp = 0;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests empty input
    str = "";
    exp = 0;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    // tests NULL input
    str = NULL;
    exp = 0;
    if (atoi(str) != exp) {
        puts("invalid conversion");
        return 1;
    }

    puts("all atoi tests passed!");
    return 0;
}

int main() {
    int failed = 0;
    
    failed += test_atoi();

    if (failed == 0) {
        puts("all stdlib tests passed!");
	    puts("========================");
        return 0;
    }

    printf("%i tests failed\n", failed);
    return -1;
}
