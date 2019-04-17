#ifndef RING_BUFFER_H__
#define RING_BUFFER_H__

#include <stdbool.h>
#include <stdint.h>

#define RING_BUFFER_DECLARE(_name, _size)             \
	static int16_t _name##_mempool[_size];            \
	static struct ring_buffer _name##_ring_buffer = { \
		.buffer = _name##_mempool,                    \
		.buffer_size = _size}

#define RING_BUFFER_IS_FULL(_name) ring_buffer_is_full(&_name##_ring_buffer)

#define RING_BUFFER_IS_EMPTY(_name) ring_buffer_is_empty(&_name##_ring_buffer)

#define RING_BUFFER_PUSH(_name, _val) ring_buffer_push(&_name##_ring_buffer, _val)

#define RING_BUFFER_POP(_name, _data) ring_buffer_pop(&_name##_ring_buffer, _data)

#define RING_BUFFER_FORCE_PUSH(_name, _val) ring_buffer_force_push(&_name##_ring_buffer, _val)

struct ring_buffer {
	int16_t *buffer;
	uint8_t index_head;
	uint8_t index_tail;
	uint8_t buffer_size;
};

/**
 *
 * @param self
 * @return
 */
static inline bool ring_buffer_is_full(struct ring_buffer *self)
{
	return self->index_tail == (self->index_head + 1) % self->buffer_size;
}

/**
 *
 * @param self
 * @return
 */
static inline bool ring_buffer_is_empty(struct ring_buffer *self)
{
	return self->index_head == self->index_tail;
}

/**
 *
 * @param self
 * @param value
 * @return
 */
static inline bool ring_buffer_push(struct ring_buffer *self, int16_t value)
{
	bool ret = false;
	if (!ring_buffer_is_full(self)) {
		self->buffer[self->index_head] = value;
		self->index_head = (self->index_head + 1) % self->buffer_size;
		ret = true;
	}
	return ret;
}

/**
 *
 * @param self
 * @param value
 */
static inline void ring_buffer_force_push(struct ring_buffer *self, int16_t value)
{
	self->buffer[self->index_head] = value;
	if (ring_buffer_is_full(self)) {
		self->index_tail = (self->index_tail + 1) % self->buffer_size;
	}
	self->index_head = (self->index_head + 1) % self->buffer_size;
}

/**
 *
 * @param self
 * @param data
 * @return
 */
static inline bool ring_buffer_pop(struct ring_buffer *self, int16_t *data)
{
	bool ret = false;
	if (!ring_buffer_is_empty(self)) {
		*data = self->buffer[self->index_tail];
		self->index_tail = (self->index_tail + 1) % self->buffer_size;
		ret = true;
	}
	return ret;
}

#endif	/*RING_BUFFER_H__*/
