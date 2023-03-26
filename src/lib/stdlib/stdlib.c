#include "stdlib.h"

int atoi( const char *str ) {

	if (str == NULL) return 0; // handles NULL edge case

	int retval = 0; // converted integer
	int sign = 1;	// sign

	for (; *str == ' '; str++); // skips spaces

	// handles sign at beginning of number
	if (*str == '-') {
		sign *= -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	// handles inserting all valid characters
	for (; *str != '\0' && *str >= '0' && *str <= '9'; str++) { 
		retval = (retval * 10) + (*str - '0');
	}

	// invalid characters are ignored
	// sign is calculated at the end
	return sign * retval;
}
