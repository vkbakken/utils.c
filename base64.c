#include "utils/base64.h"

static const unsigned char base64_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_encode(const unsigned char *src, size_t len, unsigned char *dst)
{
	unsigned char *pos;
	const unsigned char *end, *in;
	size_t out_len;
	int line_len;

	pos = dst;
	if (pos == NULL || src == NULL)
		return DATA_POINTER_INVALID;

	out_len = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	out_len += out_len / 72;   /* line feeds */
	out_len++;				   /* nul termination */
	if (out_len < len)
		return INPUT_LENGTH_INVALID; /* integer overflow */

	end = src + len;
	in = src;
	line_len = 0;
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
		line_len += 4;
		if (line_len >= 72) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	*pos = '\0';
	out_len = pos - dst;

	return out_len;
}

int base64_decode(const unsigned char *src, size_t len, unsigned char *dst)
{
	unsigned char dtable[256], *pos, block[4], tmp;
	size_t i, count, out_len;
	int pad = 0;

	pos = dst;
	if (pos == NULL || src == NULL)
		return DATA_POINTER_INVALID;

	memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (unsigned char)i;
	dtable['='] = 0;

	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[src[i]] != 0x80)
			count++;
	}

	if (count == 0 || count % 4)
		return INPUT_LENGTH_INVALID;

	out_len = count / 4 * 3;

	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[src[i]];
		if (tmp == 0x80)
			continue;

		if (src[i] == '=')
			pad++;
		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
			if (pad) {
				if (pad == 1)
					pos--;
				else if (pad == 2)
					pos -= 2;
				else {
					return NULL;
				}
				break;
			}
		}
	}

	out_len = pos - dst;

	return out_len;
}