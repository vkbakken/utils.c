#ifndef AVERAGE_FILTER_H_INCLUDED
#define AVERAGE_FILTER_H_INCLUDED

#include "utils/os_assert.h"
#include "utils/ring_buffer.h"

#define MOVING_AVERAGE_DECLARE(_name, _window_size)         \
	RING_BUFFER_DECLARE(int16_t, _name, _window_size + 1); \
	static struct moving_average _name##_moving_average = { \
		.ring_buffer_p = &_name##_ring_buffer,              \
		.window_size = _window_size,                        \
		.current_sum = 0}

#define MOVING_AVERAGE_PROCESS(_name, _val) moving_avg(&_name##_moving_average, _val)

struct moving_average {
	struct ring_buffer *ring_buffer_p;
	int32_t current_sum;
	uint8_t window_size;
};

/**
 * The API used for processing moving average after adding the first element into the window buffer.
 * @param self The reference of a @ref moving_average object.
 * @param value The value will be add to the window buffer.
 * @return
 */
static inline int16_t moving_avg(struct moving_average *self, int16_t value)
{
	int16_t current_average, remove_sample;

    __ASSERT(NULL != self);

	if (ring_buffer_push(self->ring_buffer_p, &value)) {
		self->current_sum += value;
		current_average = self->current_sum / self->window_size;

		if (ring_buffer_is_full(self->ring_buffer_p)) {
			if (ring_buffer_pop(self->ring_buffer_p, &remove_sample)) {
				self->current_sum -= remove_sample;
			}
		}
	}

	return current_average;
}

/**
 * The API used for processing moving average after the window buffer has been filled.
 * @param self The reference of a @ref moving_average object.
 * @param value The value will be add to the window buffer.
 * @return
 */
static inline int16_t moving_avg1(struct moving_average *self, int16_t value)
{
	int16_t current_average, remove_sample;

    __ASSERT(NULL != self);

	if (ring_buffer_push(self->ring_buffer_p, &value)) {
		self->current_sum += value;
		if (ring_buffer_is_full(self->ring_buffer_p)) {
			current_average = self->current_sum / self->window_size;
			if (ring_buffer_pop(self->ring_buffer_p, &remove_sample)) {
				self->current_sum -= remove_sample;
			}
		}
	}

	return current_average;
}

#endif /*AVERAGE_FILTER_H_INCLUDED*/