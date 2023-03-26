#include "../stdio.h"
#include <stdio.h>

int test_sn_printf() {
    int bufsize = 20;
    char buffer[bufsize];
    char *expected;

    // testing generic sn_printf input
    expected = "Hello!\n";
    sn_printf(buffer, bufsize, "%s\n", "Hello!");
    if (strncmp(buffer, expected, bufsize) != 0) {
        printf("%s : %s\n", buffer, expected);
        puts("invalid sn_printf 1");
        return 1;
    }

    // testing empty string
    expected = "empty: ";
    sn_printf(buffer, bufsize, "empty: %s", "");
    if (strncmp(buffer, expected, bufsize) != 0) {
        puts("invalid sn_printf 2");
        return 1;
    }

    return 0;
}

int main() {
    int failed = 0;
    
    failed += test_sn_printf();

    if (failed == 0) {
        puts("all stdio tests passed!");
	puts("========================");
        return 0;
    }

    printf("%i tests failed\n", failed);
    return -1;
}

