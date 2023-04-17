#include "stdlib.h"
#include "string.h"

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
void itoa_s( int in, char *buf, int bufsz ) {
	int digits;	// number of digits in i
	int idx;	// index to insert into buffer
	int mod;	// result of mod operation
	int digit;	// int value at digit
	char c;		// character of d_val

	digits = 0;

	if (bufsz == 0 || bufsz == 1) {
		return;
	}
	
	if (in == 0) {
		digits++;
	} 
	
	else if ( in < 0 ) {
		*buf = '-';
		buf++;
		bufsz--;
		in *= -1;
	}

	// finds the number of digits in the integer value
	for (int i = in; i >= 1; i /= 10, digits++)
		;

	int modv, prev;
	modv = 10;
	prev = 0;

	char temp[digits+1];

	for (int i = 0; i < digits; i++, modv *= 10) {
		idx = digits - i - 1;					// start at last idx and work backwards
		mod = (in % modv);						// take the mod of in
		digit = (mod - prev) / (modv / 10);		// get the digit from mod computation
		c = digit + '0';						// convert digit to character
		temp[idx] = c; 							// store character in buffer
		prev = mod;								// store the mod for next digit computation
	}

	if (bufsz != 0)
		temp[digits] = '\0';

	strcpy_s(buf, bufsz, temp);
}
