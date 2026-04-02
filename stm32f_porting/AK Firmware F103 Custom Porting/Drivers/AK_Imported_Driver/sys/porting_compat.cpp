#include <cstring>

#include "main.h"
#include "app.h"
#include "app_eeprom.h"
#include "app_flash.h"
#include "buzzer.h"
#include "button.h"
#include "eeprom.h"
#include "flash.h"
#include "io_cfg.h"
#include "led.h"
#include "platform.h"
#include "qrcode.h"
#include "xprintf.h"
#include "HardwareSerial.h"
#include "SPI.h"
#include "sys_boot.h"
#include "sys_cfg.h"
#include "sys_ctrl.h"

extern "C" {

system_info_t system_info;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi1;

uint8_t _start_boot_share_data_flash[sizeof(sys_boot_t)] = {0};

static uint32_t soft_watchdog_limit_ticks = 0;
static uint32_t soft_watchdog_counter_ticks = 0;
static uint8_t soft_watchdog_enabled = 0;
static uint8_t shell_use_blocking_output = 1;
static uint8_t eeprom_storage[EEPROM_END_ADDR];

extern uint32_t _etext;

}

SPIClass SPI;
HardwareSerial Serial2;

extern "C" {

static void init_gpio_output(GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_InitTypeDef gpio_init = {0};
	gpio_init.Pin = pin;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(port, &gpio_init);
}

static void init_gpio_input_pullup(GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_InitTypeDef gpio_init = {0};
	gpio_init.Pin = pin;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(port, &gpio_init);
}

static uint8_t gpio_read_active_low(GPIO_TypeDef* port, uint16_t pin)
{
	return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) ? 0U : 1U;
}

static void gpio_set(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state)
{
	HAL_GPIO_WritePin(port, pin, state);
}

static void enable_dwt_cycle_counter(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0U;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t get_flash_length_bytes(void)
{
	return 64U * 1024U;
}

static uint32_t normalize_flash_address(uint32_t address)
{
	if (address < FLASH_BASE) {
		return FLASH_BASE + address;
	}
	return address;
}

static uint8_t is_internal_flash_address(uint32_t address)
{
	uint32_t normalized = normalize_flash_address(address);
	return (normalized >= FLASH_BASE) && (normalized < (FLASH_BASE + get_flash_length_bytes()));
}

static void flash_program_bytes(uint32_t address, const uint8_t* data, uint32_t length)
{
	uint32_t flash_address = normalize_flash_address(address);

	if (!is_internal_flash_address(address)) {
		if (address == (uint32_t)_start_boot_share_data_flash) {
			std::memcpy(_start_boot_share_data_flash, data, length);
		}
		return;
	}

	HAL_FLASH_Unlock();

	for (uint32_t index = 0; index < length; index += 2U) {
		uint16_t half_word = 0xFFFFU;
		std::memcpy(&half_word, data + index, ((length - index) >= 2U) ? 2U : 1U);
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_address + index, half_word) != HAL_OK) {
			break;
		}
	}

	HAL_FLASH_Lock();
}

static void flash_erase_pages(uint32_t address, uint32_t length)
{
	uint32_t normalized_address = normalize_flash_address(address);
	if (!is_internal_flash_address(address)) {
		if (address == (uint32_t)_start_boot_share_data_flash) {
			std::memset(_start_boot_share_data_flash, 0xFF, sizeof(_start_boot_share_data_flash));
		}
		return;
	}

	uint32_t start_page = (normalized_address - FLASH_BASE) / FLASH_PAGE_SIZE;
	uint32_t page_count = (length + FLASH_PAGE_SIZE - 1U) / FLASH_PAGE_SIZE;
	FLASH_EraseInitTypeDef erase_init = {0};
	uint32_t page_error = 0U;

	erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_init.PageAddress = FLASH_BASE + (start_page * FLASH_PAGE_SIZE);
	erase_init.NbPages = page_count;

	HAL_FLASH_Unlock();
	(void)HAL_FLASHEx_Erase(&erase_init, &page_error);
	HAL_FLASH_Lock();
}

void platform_enter_critical(void)
{
	__disable_irq();
}

void platform_exit_critical(void)
{
	__enable_irq();
}

void button_init(button_t* button,
				uint16_t long_press_threshold,
				uint8_t id,
				button_init_func_t init,
				button_read_func_t read,
				button_callback_t callback)
{
	if (button == NULL) {
		return;
	}

	std::memset(button, 0, sizeof(*button));
	button->id = id;
	button->long_press_threshold = long_press_threshold;
	button->init = init;
	button->read = read;
	button->callback = callback;
	button->state = BUTTON_SW_STATE_IDLE;
	button->last_level = 1U;

	if (button->init != NULL) {
		button->init();
	}
}

void button_enable(button_t* button)
{
	if (button != NULL) {
		button->enable = 1U;
	}
}

void button_disable(button_t* button)
{
	if (button != NULL) {
		button->enable = 0U;
	}
}

void button_timer_polling(button_t* button)
{
	if ((button == NULL) || (button->enable == 0U) || (button->read == NULL) || (button->callback == NULL)) {
		return;
	}

	uint8_t level = button->read();
	if ((level == 0U) && (button->last_level != 0U)) {
		button->state = BUTTON_SW_STATE_PRESSED;
		button->hold_ticks = 0U;
		button->callback(button);
	} else if (level == 0U) {
		button->hold_ticks++;
		if ((button->hold_ticks >= button->long_press_threshold) && (button->state != BUTTON_SW_STATE_LONG_PRESSED)) {
			button->state = BUTTON_SW_STATE_LONG_PRESSED;
			button->callback(button);
		}
	} else if (button->last_level == 0U) {
		button->state = BUTTON_SW_STATE_RELEASED;
		button->callback(button);
		button->state = BUTTON_SW_STATE_IDLE;
		button->hold_ticks = 0U;
	}

	button->last_level = level;
}

void led_init(led_t* led,
			  led_init_func_t init,
			  led_action_func_t on,
			  led_action_func_t off)
{
	if (led == NULL) {
		return;
	}

	std::memset(led, 0, sizeof(*led));
	led->init = init;
	led->on = on;
	led->off = off;

	if (led->init != NULL) {
		led->init();
	}
}

void led_on(led_t* led)
{
	if ((led != NULL) && (led->on != NULL)) {
		led->on();
		led->state = 1U;
	}
}

void led_off(led_t* led)
{
	if ((led != NULL) && (led->off != NULL)) {
		led->off();
		led->state = 0U;
}
}

void led_toggle(led_t* led)
{
	if (led == NULL) {
		return;
	}

	if (led->state == 0U) {
		led_on(led);
	} else {
		led_off(led);
	}
}

uint8_t io_button_mode_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	init_gpio_input_pullup(BUTTON_MODE_IO_PORT, BUTTON_MODE_IO_PIN);
	return 0U;
}

uint8_t io_button_up_init(void)
{
	init_gpio_input_pullup(BUTTON_UP_IO_PORT, BUTTON_UP_IO_PIN);
	return 0U;
}

uint8_t io_button_down_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	init_gpio_input_pullup(BUTTON_DOWN_IO_PORT, BUTTON_DOWN_IO_PIN);
	return 0U;

}

uint8_t io_button_mode_read(void)
{
	return gpio_read_active_low(BUTTON_MODE_IO_PORT, BUTTON_MODE_IO_PIN);
}

uint8_t io_button_up_read(void)
{
	return gpio_read_active_low(BUTTON_UP_IO_PORT, BUTTON_UP_IO_PIN);
}

uint8_t io_button_down_read(void)
{
	return gpio_read_active_low(BUTTON_DOWN_IO_PORT, BUTTON_DOWN_IO_PIN);
}

void led_life_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	init_gpio_output(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN);
}

void led_life_on(void)
{
	gpio_set(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN, GPIO_PIN_SET);
}

void led_life_off(void)
{
	gpio_set(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN, GPIO_PIN_RESET);
}

void flash_io_ctrl_init(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	gpio_init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_init);

	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7U;
	(void)HAL_SPI_Init(&hspi1);

	gpio_init.Pin = FLASH_CE_IO_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(FLASH_CE_IO_PORT, &gpio_init);
	flash_cs_high();
}

void flash_cs_low(void)
{
	gpio_set(FLASH_CE_IO_PORT, FLASH_CE_IO_PIN, GPIO_PIN_RESET);
}

void flash_cs_high(void)
{
	gpio_set(FLASH_CE_IO_PORT, FLASH_CE_IO_PIN, GPIO_PIN_SET);
}

uint8_t flash_transfer(uint8_t data)
{
	uint8_t rx = 0U;
	(void)HAL_SPI_TransmitReceive(&hspi1, &data, &rx, 1U, HAL_MAX_DELAY);
	return rx;
}

void nrf24l01_io_ctrl_init(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_init.Pin = NRF_CE_IO_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(NRF_CE_IO_PORT, &gpio_init);

	gpio_init.Pin = NRF_CSN_IO_PIN;
	HAL_GPIO_Init(NRF_CSN_IO_PORT, &gpio_init);

	gpio_init.Pin = NRF_IRQ_IO_PIN;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(NRF_IRQ_IO_PORT, &gpio_init);
}

void nrf24l01_spi_ctrl_init(void)
{
	flash_io_ctrl_init();
}

void nrf24l01_ce_low(void)
{
	gpio_set(NRF_CE_IO_PORT, NRF_CE_IO_PIN, GPIO_PIN_RESET);
}

void nrf24l01_ce_high(void)
{
	gpio_set(NRF_CE_IO_PORT, NRF_CE_IO_PIN, GPIO_PIN_SET);
}

void nrf24l01_csn_low(void)
{
	gpio_set(NRF_CSN_IO_PORT, NRF_CSN_IO_PIN, GPIO_PIN_RESET);
}

void nrf24l01_csn_high(void)
{
	gpio_set(NRF_CSN_IO_PORT, NRF_CSN_IO_PIN, GPIO_PIN_SET);
}

uint8_t nrf24l01_spi_rw(uint8_t data)
{
	return flash_transfer(data);
}

void io_cfg_adc1(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();

	gpio_init.Pin = BAT_ADC_PIN;
	gpio_init.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(BAT_ADC_PORT, &gpio_init);

	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1U;
	(void)HAL_ADC_Init(&hadc1);
}

void adc_bat_io_cfg(void)
{
	ADC_ChannelConfTypeDef channel_config = {0};
	channel_config.Channel = ADC_CHANNEL_0;
	channel_config.Rank = ADC_REGULAR_RANK_1;
	channel_config.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	(void)HAL_ADC_ConfigChannel(&hadc1, &channel_config);
}

uint16_t adc_bat_io_read(uint8_t sample_count)
{
	uint32_t total = 0U;

	for (uint8_t index = 0U; index < sample_count; index++) {
		(void)HAL_ADC_Start(&hadc1);
		(void)HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		total += HAL_ADC_GetValue(&hadc1);
		(void)HAL_ADC_Stop(&hadc1);
	}

	if (sample_count == 0U) {
		return 0U;
	}

	return (uint16_t)(total / sample_count);
}

void ssd1306_clk_input_mode(void) {}
void ssd1306_clk_output_mode(void) {}
void ssd1306_clk_digital_write_low(void) {}
void ssd1306_clk_digital_write_high(void) {}
int ssd1306_clk_digital_read(void) { return 0; }
void ssd1306_data_input_mode(void) {}
void ssd1306_data_output_mode(void) {}
void ssd1306_data_digital_write_low(void) {}
void ssd1306_data_digital_write_high(void) {}
int ssd1306_data_digital_read(void) { return 0; }

void io_uart2_cfg(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	gpio_init.Pin = USART2_TX_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_init);

	gpio_init.Pin = USART2_RX_PIN;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpio_init);
}

void io_uart_rs485_cfg(void)
{
	GPIO_InitTypeDef gpio_init = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	gpio_init.Pin = GPIO_PIN_1;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_init);
}

void internal_flash_unlock(void)
{
	HAL_FLASH_Unlock();
}

void internal_flash_lock(void)
{
	HAL_FLASH_Lock();
}

void internal_flash_erase_pages_cal(uint32_t address, uint32_t length)
{
	flash_erase_pages(address, length);
}

void internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t length)
{
	flash_program_bytes(address, data, length);
}

void flash_read(uint32_t address, uint8_t* data, uint32_t length)
{
	uint32_t normalized_address = normalize_flash_address(address);
	if (is_internal_flash_address(address)) {
		std::memcpy(data, reinterpret_cast<const void*>(normalized_address), length);
		return;
	}

	if (address == (uint32_t)_start_boot_share_data_flash) {
		std::memcpy(data, _start_boot_share_data_flash, length);
		return;
	}

	std::memset(data, 0xFF, length);
}

void flash_write(uint32_t address, uint8_t* data, uint32_t length)
{
	flash_program_bytes(address, data, length);
}

void flash_erase_sector(uint32_t address)
{
	flash_erase_pages(address, FLASH_PAGE_SIZE);
}

void flash_erase_block_64k(uint32_t address)
{
	flash_erase_pages(address, FLASH_BLOCK_64K_SIZE);
}

void eeprom_read(uint32_t address, uint8_t* data, uint32_t length)
{
	if ((address + length) <= sizeof(eeprom_storage)) {
		std::memcpy(data, &eeprom_storage[address], length);
		return;
	}

	std::memset(data, 0xFF, length);
}

void eeprom_write(uint32_t address, const uint8_t* data, uint32_t length)
{
	if ((address + length) <= sizeof(eeprom_storage)) {
		std::memcpy(&eeprom_storage[address], data, length);
	}
}

void eeprom_erase(uint32_t address, uint32_t length)
{
	if ((address + length) <= sizeof(eeprom_storage)) {
		std::memset(&eeprom_storage[address], 0xFF, length);
	}
}

const buzzer_tone_t tones_startup[] = {
	{ 2200U, 80U },
	{ 3300U, 80U },
	{ 0U, 0U },
};

const buzzer_tone_t tones_3beep[] = {
	{ 2200U, 50U },
	{ 2200U, 50U },
	{ 2200U, 50U },
	{ 0U, 0U },
};

const buzzer_tone_t tones_SMB[] = {
	{ 262U, 120U },
	{ 0U, 40U },
	{ 330U, 120U },
	{ 0U, 40U },
	{ 392U, 120U },
	{ 0U, 0U },
};

const buzzer_tone_t tones_merryChrismast[] = {
	{ 523U, 120U },
	{ 659U, 120U },
	{ 784U, 120U },
	{ 0U, 0U },
};

void BUZZER_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	init_gpio_output(BUZZER_IO_PORT, BUZZER_IO_PIN);
	gpio_set(BUZZER_IO_PORT, BUZZER_IO_PIN, GPIO_PIN_RESET);
}

void BUZZER_PlayTones(const buzzer_tone_t* tones)
{
	if (tones == NULL) {
		return;
	}

	for (uint32_t index = 0U; tones[index].duration_ms != 0U || tones[index].frequency_hz != 0U; index++) {
		if (tones[index].frequency_hz != 0U) {
			gpio_set(BUZZER_IO_PORT, BUZZER_IO_PIN, GPIO_PIN_SET);
		}
		HAL_Delay(tones[index].duration_ms);
		gpio_set(BUZZER_IO_PORT, BUZZER_IO_PIN, GPIO_PIN_RESET);
		if (tones[index].duration_ms == 0U) {
			break;
		}
	}
}

void sys_cfg_clock(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	enable_dwt_cycle_counter();
	system_info.cpu_clock = HAL_RCC_GetHCLKFreq();
}

void sys_cfg_tick(void)
{
	HAL_NVIC_SetPriority(SysTick_IRQn, 15U, 0U);
}

void sys_cfg_console(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();

	gpio_init.Pin = USART1_TX_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_init);

	gpio_init.Pin = USART1_RX_PIN;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpio_init);

	huart1.Instance = USART1;
	huart1.Init.BaudRate = SYS_CONSOLE_BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	(void)HAL_UART_Init(&huart1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	HAL_NVIC_SetPriority(USART1_IRQn, IRQ_PRIO_UART0_CONSOLE, 0U);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	xdev_out(sys_ctrl_shell_put_char_block);
	system_info.console_baudrate = SYS_CONSOLE_BAUDRATE;
}

void sys_cfg_update_info(void)
{
	extern uint32_t _etext;
	extern uint32_t _sdata;
	extern uint32_t _edata;
	extern uint32_t _sbss;
	extern uint32_t _ebss;
	extern uint32_t _estack;
	extern uint32_t _end;

	system_info.flash_used = (uint32_t)&_etext - FLASH_BASE;
	system_info.ram_used = (uint32_t)&_estack - (uint32_t)&_sdata;
	system_info.data_init_size = (uint32_t)&_edata - (uint32_t)&_sdata;
	system_info.data_non_init_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	system_info.stack_avail = (uint32_t)&_estack - (uint32_t)&_ebss;
	system_info.heap_avail = (uint32_t)&_estack - (uint32_t)&_end;
	system_info.ram_other = 0U;
}

void sys_cfg_init(void)
{
	sys_cfg_clock();
	sys_cfg_tick();
	sys_cfg_console();
	sys_cfg_update_info();
}

void sys_ctrl_reset(void)
{
	NVIC_SystemReset();
}

void sys_ctrl_independent_watchdog_init(void)
{
	__HAL_RCC_LSI_ENABLE();
	while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET) {
	}

	IWDG->KR = 0x5555U;
	IWDG->PR = 0x06U;
	IWDG->RLR = 4095U;
	while (IWDG->SR != 0U) {
	}
	IWDG->KR = 0xCCCCU;
	IWDG->KR = 0xAAAAU;
}

void sys_ctrl_independent_watchdog_reset(void)
{
	IWDG->KR = 0xAAAAU;
}

void sys_ctrl_soft_watchdog_init(uint32_t timeout_ticks)
{
	soft_watchdog_limit_ticks = timeout_ticks;
	soft_watchdog_counter_ticks = 0U;
	soft_watchdog_enabled = 1U;
}

void sys_ctrl_soft_watchdog_reset(void)
{
	soft_watchdog_counter_ticks = 0U;
}

void sys_ctrl_soft_watchdog_enable(void)
{
	soft_watchdog_enabled = 1U;
}

void sys_ctrl_soft_watchdog_disable(void)
{
	soft_watchdog_enabled = 0U;
}

void sys_ctrl_soft_watchdog_increase_counter(void)
{
	if ((soft_watchdog_enabled != 0U) && (soft_watchdog_limit_ticks != 0U)) {
		soft_watchdog_counter_ticks++;
		if (soft_watchdog_counter_ticks >= soft_watchdog_limit_ticks) {
			sys_ctrl_reset();
		}
	}
}

void sys_ctrl_delay(volatile uint32_t count)
{
	while (count-- != 0U) {
		__NOP();
	}
}

void sys_ctrl_delay_ms(volatile uint32_t count)
{
	HAL_Delay(count);
}

void sys_ctrl_delay_us(volatile uint32_t count)
{
	uint32_t start = DWT->CYCCNT;
	uint32_t cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
	uint32_t target = count * cycles_per_us;
	while ((DWT->CYCCNT - start) < target) {
	}
}

uint32_t sys_ctrl_micros(void)
{
	uint32_t cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
	if (cycles_per_us == 0U) {
		return 0U;
	}
	return DWT->CYCCNT / cycles_per_us;
}

uint32_t sys_ctrl_millis(void)
{
	return HAL_GetTick();
}

uint8_t sys_ctrl_shell_get_char(void)
{
	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == RESET) {
		return 0U;
	}
	return (uint8_t)(huart1.Instance->DR & 0xFFU);
}

void sys_ctrl_shell_put_char(uint8_t c)
{
	if (shell_use_blocking_output == 0U) {
		/* keep the API stable even though the port uses a blocking console */
	}
	(void)HAL_UART_Transmit(&huart1, &c, 1U, HAL_MAX_DELAY);
}

void sys_ctrl_shell_put_char_block(uint8_t c)
{
	(void)HAL_UART_Transmit(&huart1, &c, 1U, HAL_MAX_DELAY);
}

void sys_ctrl_shell_sw_to_block(void)
{
	shell_use_blocking_output = 1U;
	xdev_out(sys_ctrl_shell_put_char_block);
}

void sys_ctrl_shell_sw_to_nonblock(void)
{
	shell_use_blocking_output = 0U;
	xdev_out(sys_ctrl_shell_put_char);
}

void sys_ctrl_get_firmware_info(firmware_header_t* firmware_header)
{
	if (firmware_header == NULL) {
		return;
	}

	firmware_header->psk = FIRMWARE_PSK;
	firmware_header->bin_len = (uint32_t)&_etext - FLASH_BASE;
	firmware_header->checksum = 0U;
}

void sys_ctr_sleep_wait_for_irq(void)
{
	__WFI();
}

uint32_t sys_ctr_get_exception_number(void)
{
	return __get_IPSR();
}

void sys_ctr_restart_app(void)
{
	sys_ctrl_reset();
}

uint32_t sys_ctr_get_vbat_voltage(void)
{
	uint16_t adc = adc_bat_io_read(4U);
	return (uint32_t)((adc * 3300U) / 4095U);
}

uint32_t sys_ctr_get_mcu_temperature(void)
{
	return 25U;
}

void sys_ctr_stop_mcu(void)
{
	HAL_PWR_EnterSTANDBYMode();
}

int asm_test_add(int lhs, int rhs)
{
	return lhs + rhs;
}

uint32_t sys_stack_count_init(void)
{
	return sys_stack_get_size();
}

uint32_t sys_stack_usage(void)
{
	extern uint32_t _estack;
	uint32_t current_sp = __get_MSP();
	return (uint32_t)&_estack - current_sp;
}

uint32_t sys_stack_get_size(void)
{
	extern uint32_t _estack;
	extern uint32_t _ebss;
	return (uint32_t)&_estack - (uint32_t)&_ebss;
}

void sys_irq_nrf24l01(void)
{
}

void sys_irq_uart2(void)
{
}

} // extern "C"