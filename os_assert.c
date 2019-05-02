/*
 * os_assert.h
 *
 *  Created on: Apr 25, 2019
 *      Author: liemlb
 */

#include "utils/ring_buffer.h"

struct assert_fault {
	const char *expression;
	const char *filename;
	int line;
};

RING_BUFFER_DECLARE(struct assert_fault, assert_log, 10);

void __assert(const char *__expression, const char *__filename, int __line)
{
	struct assert_fault tmp = {
		.expression = __expression,
		.filename = __filename,
		.line = __line};

	RING_BUFFER_PUSH(assert_log, &tmp);

	while (RING_BUFFER_IS_FULL(assert_log)) {
	}
}