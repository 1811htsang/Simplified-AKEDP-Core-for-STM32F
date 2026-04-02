#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef void (*led_init_func_t)(void);
typedef void (*led_action_func_t)(void);

typedef struct {
	uint8_t state;
	led_init_func_t init;
	led_action_func_t on;
	led_action_func_t off;
} led_t;

void led_init(led_t* led,
			  led_init_func_t init,
			  led_action_func_t on,
			  led_action_func_t off);
void led_on(led_t* led);
void led_off(led_t* led);
void led_toggle(led_t* led);

#ifdef __cplusplus
}
#endif

#endif