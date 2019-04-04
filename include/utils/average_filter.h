#ifndef AVERAGE_FILTER_H__
#define AVERAGE_FILTER_H__

#include "utils/ring_buffer.h"

#define MOVING_AVERAGE_DECLARE(_name, _window_size)   \
static int16_t _name##_buffer[_window_size];\
static struct ring_buffer_handler _name##_ring_buffer_handler = {\
    .buffer = _name##_buffer,\
    .buffer_size = _window_size\
};\
static struct moving_average_handler _name##_moving_average_handler = {\
    .rp_handler_p = &_name##_ring_buffer_handler,\
}

#define MOVING_AVERAGE_PROCESS(_name, _val) moving_avg(&_name##_moving_average_handler, _val)

struct moving_average_handler{
    struct ring_buffer_handler *rp_handler_p;
    int32_t current_sum;
};

static inline int16_t moving_avg(struct moving_average_handler *ma_handler_p, int16_t value){
    int16_t current_average, remove_sample;

    if(ring_buffer_push(ma_handler_p->rp_handler_p, value)){
        ma_handler_p->current_sum += value;
        current_average = ma_handler_p->current_sum / ma_handler_p->rp_handler_p->buffer_size;

        if(ma_handler_p->rp_handler_p->full){            
            if(ring_buffer_pop(ma_handler_p->rp_handler_p, &remove_sample)){
                ma_handler_p->current_sum -= remove_sample;
            }
        }            
    }

    return current_average;
}

static inline int16_t moving_avg1(struct moving_average_handler *ma_handler_p, int16_t value){
    int16_t current_average, remove_sample;

    if(ring_buffer_push(ma_handler_p->rp_handler_p, value)){
        ma_handler_p->current_sum += value;
        if(ma_handler_p->rp_handler_p->full){
            current_average = ma_handler_p->current_sum / ma_handler_p->rp_handler_p->buffer_size;
            if(ring_buffer_pop(ma_handler_p->rp_handler_p, &remove_sample)){
                ma_handler_p->current_sum -= remove_sample;
            }
        }            
    }

    return current_average;
}

#endif
