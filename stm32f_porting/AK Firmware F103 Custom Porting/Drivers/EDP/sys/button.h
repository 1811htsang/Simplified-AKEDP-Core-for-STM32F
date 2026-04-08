#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef enum {
	BUTTON_SW_STATE_IDLE = 0,
	BUTTON_SW_STATE_PRESSED,
	BUTTON_SW_STATE_LONG_PRESSED,
	BUTTON_SW_STATE_RELEASED,
} button_state_t;

typedef void (*button_callback_t)(void*);
typedef uint8_t (*button_init_func_t)(void);
typedef uint8_t (*button_read_func_t)(void);

typedef struct {
	uint8_t id;
	uint8_t enable;
	uint8_t state;
	uint8_t last_level;
	uint16_t long_press_threshold;
	uint16_t hold_ticks;
	button_init_func_t init;
	button_read_func_t read;
	button_callback_t callback;
} button_t;

void button_init(button_t* button,
				uint16_t long_press_threshold,
				uint8_t id,
				button_init_func_t init,
				button_read_func_t read,
				button_callback_t callback);
void button_enable(button_t* button);
void button_disable(button_t* button);
void button_timer_polling(button_t* button);

#ifdef __cplusplus
}
#endif

#endif