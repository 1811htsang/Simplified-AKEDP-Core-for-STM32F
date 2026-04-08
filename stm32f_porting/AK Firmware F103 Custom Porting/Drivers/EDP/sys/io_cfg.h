#ifndef __IO_CFG_H__
#define __IO_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "main.h"

/* Active board control pins used by the current AK-EDP core. */
#define BUTTON_MODE_IO_PIN GPIO_PIN_4
#define BUTTON_MODE_IO_PORT GPIOB

#define BUTTON_UP_IO_PIN GPIO_PIN_13
#define BUTTON_UP_IO_PORT GPIOC

#define BUTTON_DOWN_IO_PIN GPIO_PIN_3
#define BUTTON_DOWN_IO_PORT GPIOB

#define LED_LIFE_IO_PIN GPIO_PIN_8
#define LED_LIFE_IO_PORT GPIOB

#define BUZZER_IO_PIN GPIO_PIN_0
#define BUZZER_IO_PORT GPIOB

#define FLASH_CE_IO_PIN GPIO_PIN_14
#define FLASH_CE_IO_PORT GPIOB

#define NRF_CE_IO_PIN GPIO_PIN_8
#define NRF_CE_IO_PORT GPIOA
#define NRF_CSN_IO_PIN GPIO_PIN_9
#define NRF_CSN_IO_PORT GPIOB
#define NRF_IRQ_IO_PIN GPIO_PIN_1
#define NRF_IRQ_IO_PORT GPIOB

#define BAT_ADC_PIN GPIO_PIN_0
#define BAT_ADC_PORT GPIOA

#define USART1_TX_PIN GPIO_PIN_9
#define USART1_RX_PIN GPIO_PIN_10
#define USART2_TX_PIN GPIO_PIN_2
#define USART2_RX_PIN GPIO_PIN_3

/* GPIO input helpers for board buttons. */
uint8_t io_button_mode_init(void);
uint8_t io_button_up_init(void);
uint8_t io_button_down_init(void);
uint8_t io_button_mode_read(void);
uint8_t io_button_up_read(void);
uint8_t io_button_down_read(void);

/* Life LED control. */
void led_life_init(void);
void led_life_on(void);
void led_life_off(void);

/* External flash control. */
void flash_io_ctrl_init(void);
void flash_cs_low(void);
void flash_cs_high(void);
uint8_t flash_transfer(uint8_t data);

/* ADC helper path for battery monitoring. */
void io_cfg_adc1(void);
void adc_bat_io_cfg(void);
uint16_t adc_bat_io_read(uint8_t sample_count);

/* SSD1306 bit-banged display bus helpers. */
void ssd1306_clk_input_mode(void);
void ssd1306_clk_output_mode(void);
void ssd1306_clk_digital_write_low(void);
void ssd1306_clk_digital_write_high(void);
int ssd1306_clk_digital_read(void);
void ssd1306_data_input_mode(void);
void ssd1306_data_output_mode(void);
void ssd1306_data_digital_write_low(void);
void ssd1306_data_digital_write_high(void);
int ssd1306_data_digital_read(void);

/* UART and RS485 routing helpers. */
void io_uart2_cfg(void);
void io_uart_rs485_cfg(void);

/* Internal flash helpers used by firmware update flow. */
void internal_flash_unlock(void);
void internal_flash_lock(void);
void internal_flash_erase_pages_cal(uint32_t address, uint32_t length);
void internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif