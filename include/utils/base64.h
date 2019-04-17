#ifndef UTILS_BASE64_H_INCLUDED
#define UTILS_BASE64_H_INCLUDED


#include <stdint.h>
#include <stdbool.h>


uint16_t base64_encode(uint8_t *in, uint16_t in_size, uint8_t *out, uint16_t out_size);
#endif /*UTILS_BASE64_H_INCLUED*/
