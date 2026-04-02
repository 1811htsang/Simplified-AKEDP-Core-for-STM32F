#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"

void platform_enter_critical(void);
void platform_exit_critical(void);

#define DISABLE_INTERRUPTS() __disable_irq()
#define ENABLE_INTERRUPTS() __enable_irq()
#define ENTRY_CRITICAL() platform_enter_critical()
#define EXIT_CRITICAL() platform_exit_critical()

#ifndef LOG2LKUP
#define LOG2LKUP(x) ((uint32_t)(31U - __CLZ((uint32_t)(x))))
#endif

#ifdef __cplusplus
}
#endif

#endif