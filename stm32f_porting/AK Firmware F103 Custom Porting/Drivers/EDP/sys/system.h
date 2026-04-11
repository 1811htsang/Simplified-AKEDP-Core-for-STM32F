#ifndef __SYSTEM_H__
	#define __SYSTEM_H__

	// Khai báo directive C cho C++
	#ifdef __cplusplus
		extern "C"
		{
	#endif

	// Khai báo thư viện sử dụng
	#include <stdint.h>

	typedef enum {
		IRQ_PRIO_UART0_CONSOLE = 2,
		IRQ_PRIO_UART2 = 3,
		IRQ_PRIO_EXTI = 4,
		IRQ_PRIO_TIMER_10MS = 5,
	} system_irq_prio_e;

	typedef struct {
		uint32_t flash_used;
		uint32_t ram_used;
		uint32_t data_init_size;
		uint32_t data_non_init_size;
		uint32_t stack_avail;
		uint32_t heap_avail;
		uint32_t ram_other;
		uint32_t cpu_clock;
		uint32_t tick;
		uint32_t console_baudrate;
	} system_info_t;

	extern system_info_t system_info;

#ifdef __cplusplus
	}
#endif

#endif
