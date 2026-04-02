#ifndef __ADAFRUIT_SSD1306SYP_H__
#define __ADAFRUIT_SSD1306SYP_H__

#include <stdint.h>

#ifndef WHITE
#define WHITE 1
#endif

#ifndef BLACK
#define BLACK 0
#endif

class Adafruit_ssd1306syp {
public:
	void initialize(void) {}
	void clear(void) {}
	void update(void) {}
	void display_on(void) {}
	void display_off(void) {}
	void setTextSize(uint8_t) {}
	void setTextColor(uint8_t) {}
	void setCursor(int16_t, int16_t) {}
	void print(const char*) {}
	void drawRect(int16_t, int16_t, int16_t, int16_t, uint8_t) {}
	void fillRect(int16_t, int16_t, int16_t, int16_t, uint8_t) {}
	void fillRoundRect(int16_t, int16_t, int16_t, int16_t, int16_t, uint8_t) {}
	void drawBitmap(int16_t, int16_t, const unsigned char*, int16_t, int16_t, uint8_t) {}
	void drawPixel(int16_t, int16_t, uint8_t) {}
	void fillScreen(uint8_t) {}
	void drawCircle(int16_t, int16_t, int16_t, uint8_t) {}
	int16_t width(void) const { return 128; }
};

#endif