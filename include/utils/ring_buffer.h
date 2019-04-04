#ifndef RING_BUFFER_H__
#define RING_BUFFER_H__

#include <stdint.h>
#include <stdbool.h>

struct ring_buffer_handler{
  int16_t *buffer;
  uint8_t index_head;
  uint8_t index_tail;
  uint8_t buffer_size;
  bool full;
};

static bool ring_buffer_push(struct ring_buffer_handler *rp_handler_p, int16_t value){
	//check if out of memory before writing
	if(!rp_handler_p->full){
		rp_handler_p->buffer[rp_handler_p->index_head] = value;

		rp_handler_p->index_head = (rp_handler_p->index_head + 1) % rp_handler_p->buffer_size;
		rp_handler_p->full = (rp_handler_p->index_head == rp_handler_p->index_tail);

		return true;//added successful
	}
	return false;//out of memory, not added yet
}

static void ring_buffer_force_push(struct ring_buffer_handler *rp_handler_p, int16_t value){
    rp_handler_p->buffer[rp_handler_p->index_head] = value;

    if(rp_handler_p->full){
        rp_handler_p->index_tail = (rp_handler_p->index_tail + 1) % rp_handler_p->buffer_size;
    }
    rp_handler_p->index_head = (rp_handler_p->index_head + 1) % rp_handler_p->buffer_size;
    rp_handler_p->full = (rp_handler_p->index_head == rp_handler_p->index_tail);
}

static bool ring_buffer_pop(struct ring_buffer_handler *rp_handler_p, int16_t *data){
	if(rp_handler_p->full || (rp_handler_p->index_head != rp_handler_p->index_tail)){//check empty
		*data = rp_handler_p->buffer[rp_handler_p->index_tail];

		rp_handler_p->full = false;
		rp_handler_p->index_tail = (rp_handler_p->index_tail + 1) % rp_handler_p->buffer_size;

		return true;
	}
	return false;// buffer empty
}

#endif
