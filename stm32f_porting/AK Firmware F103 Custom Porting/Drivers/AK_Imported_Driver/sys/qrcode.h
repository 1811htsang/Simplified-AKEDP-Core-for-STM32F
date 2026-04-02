#ifndef __QRCODE_H__
#define __QRCODE_H__

#include <stdint.h>

typedef struct {
	uint8_t size;
} QRCode;

static inline uint16_t qrcode_getBufferSize(uint8_t version)
{
	return (uint16_t)(version * version * 2U + 32U);
}

static inline void qrcode_initText(QRCode* qrcode,
										  uint8_t* buffer,
										  uint8_t version,
										  int ecc,
										  const char* text)
{
	(void)buffer;
	(void)ecc;
	(void)text;
	qrcode->size = (uint8_t)(version * 4U + 17U);
}

static inline uint8_t qrcode_getModule(const QRCode* qrcode,
										   uint8_t x,
										   uint8_t y)
{
	return (uint8_t)(((x + y) & 0x01U) != 0U);
}

#endif