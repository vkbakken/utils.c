#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum BASE64_ENCODE_ERROR{
	DATA_POINTER_INVALID = -1,
	INPUT_LENGTH_INVALID = -2
};

int base64_encode(const unsigned char *src, size_t len, unsigned char *dst);
int base64_decode(const unsigned char *src, size_t len, unsigned char *dst);

#endif /*BASE64_H_INCLUDED*/
