/*
 * os_mutex.h
 *
 *  Created on: Apr 2, 2019
 *      Author: liemlb
 */

#ifndef OS_MUTEX_H_
#define OS_MUTEX_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef volatile int os_mutex_t;

enum OS_MUXTEX_STATE {
	OS_MTX_LOCKED = 1,
	OS_MTX_UNLOCKED = 0
};

#define OS_MTX_DECLARE(_name) os_mutex_t _name##_mtx = OS_MTX_UNLOCKED;

#define OS_MTX_LOCK(_name) os_mutex_lock(&_name##_mtx)

#define OS_MTX_UNLOCK(_name) os_mutex_unlock(&_name##_mtx)

static inline bool os_mutex_lock(os_mutex_t *self)
{
	assert(NULL != self);

	if (*self == OS_MTX_UNLOCKED) {
		*self = OS_MTX_LOCKED;
		__DMB();
		return true;
	}

	return false;
}

static inline bool os_mutex_unlock(os_mutex_t *self)
{
	assert(NULL != self);

	if (*self == OS_MTX_LOCKED) {
		*self = OS_MTX_UNLOCKED;
		__DMB();
		return true;
	}

	return false;
}

#endif /* OS_MUTEX_H_ */
