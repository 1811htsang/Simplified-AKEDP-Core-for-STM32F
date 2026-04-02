#ifndef __HARDWARE_SERIAL_H__
#define __HARDWARE_SERIAL_H__

#include <stddef.h>
#include <stdint.h>

class HardwareSerial {
public:
	HardwareSerial() = default;

	void begin(unsigned long baud = 9600UL) { (void)baud; }
	void setTimeout(unsigned long timeout_ms) { (void)timeout_ms; }
	int available(void) const { return 0; }
	int read(void) { return -1; }
	size_t write(uint8_t) { return 1U; }
	void flush(void) {}
};

extern HardwareSerial Serial2;

#endif