#include "../../test_helper.h"
#include "stdlib/string.h"

void test_strnlen_s(char *in, int sz, int exp) {
    ASSERT (strnlen_s(in, sz) == exp);
}

// memcpy from void *in to void *buf
void test_memcpy(void *in, int sz) {
    char buf[sz+1];
    buf[sz] = '#';

    memcpy(buf, in, sz);

    int i;
    for (i = 0; i < sz; i++) {
        ASSERT (((char *)in)[i] == ((char *)buf)[i]);
    }
    ASSERT (buf[i] == '#');
}

void test_strncmp(char *s1, char *s2, int sz, int exp) {
    int out = strncmp(s1, s2, sz);
    ASSERT (out == exp);
    // serialWrite(COM1,  &c, sizeof(int));
}

// first two arguments are direct arguments to strcpy_s
// cmpsz is the expected size (if sz is larger than the 
// actual strlen)
void test_strcpy_s(void *in, int sz, int cmpsz) {
    int tsz = cmpsz + 10;
    char buf[tsz];
    for (int i = 0; i < tsz; i++) {
        buf[i] = '#';
    }

    strcpy_s(buf, sz, in);

    int i;
    // tests that memory is copied appropriately
    for (i = 0; i < cmpsz; i++) {
        ASSERT (((char *)in)[i] == ((char *)buf)[i]);
    }

    // test that last byte is `\0`
    ASSERT (((char *)buf)[i] == '\0');

    // tests that bytes *after* '\0' are not overwritten
    for (int i = cmpsz + 1; i < tsz; i++) {
        ASSERT (buf[i] == '#');
    }
}

int test_main() {
    test_strnlen_s("makeopensource!", 100, 15);
    test_strnlen_s("", 100, 0);
    test_strnlen_s(NULL, 100, 0);
    test_strnlen_s("makeopensource!", 4, 4);
    test_strnlen_s("makeopensource!", 0, 0);

    char c = '4';
    int i = 5;
    long l = 6;

    test_memcpy("", 0);
    test_memcpy("Hello, world!", 13);
    test_memcpy(&c, sizeof(char));
    test_memcpy(&i, sizeof(int));
    test_memcpy(&l, sizeof(long));

    test_strncmp("Hello there", "General Kenobi", 100, 'H' - 'G');
    test_strncmp("Hello", "Hello, world!", 100, '\0' - ',');
    test_strncmp("Hello", "Hello, world!", 5, 'o' - 'o');
    test_strncmp("Hello\0something", "Hello\0different", 100, 'o' - 'o');
    test_strncmp("Hello world", "Hello There!", 100, 'w' - 'T');

    test_strcpy_s("", 100, 0);
    test_strcpy_s("copy", 100, 4);
    test_strcpy_s("makeopensource", 8, 8);
    test_strcpy_s("hello!", 0, 0);

    char done[] = "test_string done\n";
    serialWrite(COM1, (uint8_t*)(done), 17);

    return 0;
}
