#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <stdint.h>
#include <stdlib.h>

#ifndef PROGMEM
#define PROGMEM
#endif

static inline long random(long min_val, long max_val) {
	if (max_val <= min_val) {
		return min_val;
	}
	return min_val + (long)(rand() % (int)(max_val - min_val));
}

#endif
