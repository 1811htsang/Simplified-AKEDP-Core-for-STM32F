#ifndef __SYS_CFG_H__
#define __SYS_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "main.h"
#include "system.h"

#define SYS_CONSOLE_BAUDRATE 115200U

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi1;

void sys_cfg_init(void);
void sys_cfg_clock(void);
void sys_cfg_tick(void);
void sys_cfg_console(void);
void sys_cfg_update_info(void);

#ifdef __cplusplus
}
#endif

#endif