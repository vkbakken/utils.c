#ifndef RING_BUFFER_H_INCLUDED
#define RING_BUFFER_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "utils/os_assert.h"

#define RING_BUFFER_DECLARE(_type, _name, _size)      \
	static _type _name##_mempool[(_size)];            \
	static struct ring_buffer _name##_ring_buffer = { \
		.buffer = _name##_mempool,                    \
		.buffer_size = (_size),                       \
		.element_size = sizeof(_type)}

#define RING_BUFFER_IS_FULL(_name) ring_buffer_is_full(&_name##_ring_buffer)

#define RING_BUFFER_IS_EMPTY(_name) ring_buffer_is_empty(&_name##_ring_buffer)

#define RING_BUFFER_COUNT(_name) ring_buffer_count(&_name##_ring_buffer)

#define RING_BUFFER_PUSH(_name, _input) ring_buffer_push(&_name##_ring_buffer, _input)

#define RING_BUFFER_POP(_name, _output) ring_buffer_pop(&_name##_ring_buffer, _output)

#define RING_BUFFER_FORCE_PUSH(_name, _input) ring_buffer_force_push(&_name##_ring_buffer, _input)

struct ring_buffer {
	void *buffer;
	size_t index_head;
	size_t index_tail;
	size_t buffer_size;
	size_t element_size;
};

/**
 *
 * @param self The reference of a @ref ring_buffer object.
 * @return true if the buffer is full otherwise false.
 */
static inline bool ring_buffer_is_full(struct ring_buffer *self)
{
	__ASSERT(NULL != self);

	return self->index_tail == (self->index_head + 1) % self->buffer_size;
}

/**
 *
 * @param self The reference of a @ref ring_buffer object.
 * @return true if the buffer is empty otherwise false.
 */
static inline bool ring_buffer_is_empty(struct ring_buffer *self)
{
	__ASSERT(NULL != self);

	return self->index_head == self->index_tail;
}

/**
 *
 * @param self The reference of a @ref ring_buffer object.
 * @return true if the buffer is empty otherwise false.
 */
static inline uint8_t ring_buffer_count(struct ring_buffer *self)
{
	uint8_t count = self->buffer_size - 1;

	__ASSERT(NULL != self);

	if (!ring_buffer_is_full(self)) {
		if (self->index_head >= self->index_tail) {
			count = (self->index_head - self->index_tail);
		} else {
			count = (self->buffer_size + self->index_head - self->index_tail);
		}
	}

	return count;
}

/**
 *
 * @param self The reference of a @ref ring_buffer object.
 * @param input The data will be pushed to the buffer.
 * @return
 */
static inline bool ring_buffer_push(struct ring_buffer *self, void *input)
{
	bool ret = false;

	__ASSERT(NULL != self && NULL != input);

	if (!ring_buffer_is_full(self)) {
		memcpy(self->buffer + self->index_head * self->element_size,
			input, self->element_size);
		self->index_head = (self->index_head + 1) % self->buffer_size;
		ret = true;
	}
	return ret;
}

/**
 *
 * @param self The reference of a @ref ring_buffer object.
 * @param input The data will be pushed to the buffer.
 */
static inline void ring_buffer_force_push(struct ring_buffer *self, void *input)
{
	__ASSERT(NULL != self && NULL != input);

	memcpy(self->buffer + self->index_head * self->element_size,
		input, self->element_size);
	if (ring_buffer_is_full(self)) {
		self->index_tail = (self->index_tail + 1) % self->buffer_size;
	}
	self->index_head = (self->index_head + 1) % self->buffer_size;
}

/**
 *
 * @param self The reference of a @ref ring_buffer object.
 * @param output The place used to stored popped out data.
 * @return
 */
static inline bool ring_buffer_pop(struct ring_buffer *self, void *output)
{
	bool ret = false;

	__ASSERT(NULL != self && NULL != output);

	if (!ring_buffer_is_empty(self)) {
		memcpy(output, self->buffer + self->index_tail * self->element_size,
			self->element_size);
		self->index_tail = (self->index_tail + 1) % self->buffer_size;
		ret = true;
	}
	return ret;
}

#endif /*RING_BUFFER_H_INCLUDED*/