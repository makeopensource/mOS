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

// The math behind this approach is as follows:
//
// Taking the mod of any base 10 digit "d" and 10 gives that digit 
//     (2 % 10) = 2
//
// If we multiply the mod by ten, we get the sum of the next digit and 
// the least significant one
//     (12 % 10) = 2, (12 % 100) = 12
//
// We can get each digit by contunuing along this sequence and subtracting 
// the previous value to find the next digit
//     For 523:
//       523 % 10 = 3
//	     523 % 100 = 23; 23 - 3 = 20; 20 / 10 = 2
//       523 % 1000 = 523; 523 - 23 = 500; 500 / 10 = 5
void itoa( const int i, char *buf ) {
	int digits = 0;

	// finds the number of digits in the integer value
	for (int temp = i; temp >= 1; temp *= 0.1, digits++)
		;

	int acc, modv, prev;
	for (acc = 0, modv = 10, prev = 0; acc < digits; acc++, modv *= 10) {
		int r_idx = digits - acc - 1;				// index to insert into buffer
		int m_val = (i % modv);						// result of mod operation
		int r_val = (m_val - prev) / (modv / 10);	// int value at numerical index
		buf[r_idx] = r_val + '0';					// character value + storage
		prev = m_val;
	}
	buf[acc] = '\0';
}
