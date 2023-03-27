#include "../stdio.h"
#include <stdio.h>

int test_sn_printf() {
    int bufsize = 50;
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

    // testing empty string
    expected = "charA: a, charB: b";
    sn_printf(buffer, bufsize, "charA: %c, charB: %c", 'a', 'b');
    if (strncmp(buffer, expected, bufsize) != 0) {
        puts("invalid sn_printf 3");
        return 1;
    }

    // testing empty string
    expected = "charA: a, charB: b";
    sn_printf(buffer, bufsize, "charA: %c, charB: %c", 'a', 'b');
    if (strncmp(buffer, expected, bufsize) != 0) {
        puts("invalid sn_printf 3");
        return 1;
    }

    // testing numbers
    expected = "numbers: 12, 34";
    sn_printf(buffer, bufsize, "numbers: %i, %i", 12, 34);
    if (strncmp(buffer, expected, bufsize) != 0) {
        puts("invalid sn_printf 4");
        return 1;
    }

    // testing %% input
    expected = "We are up 112%!";
    sn_printf(buffer, bufsize, "We are up 112%%!");
    if (strncmp(buffer, expected, bufsize) != 0) {
        puts("invalid sn_printf 5");
        return 1;
    }

    // testing negative numbers
    expected = "numbers: -12, -34";
    sn_printf(buffer, bufsize, "numbers: %i, %i", -12, -34);
    if (strncmp(buffer, expected, bufsize) != 0) {
        puts("invalid sn_printf 4");
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

