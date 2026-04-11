#include <cstring>

#include "main.h"
#include "app.h"
#include "app_flash.h"
#include "flash.h"
#include "io_cfg.h"
#include "platform.h"
#include "sys_boot.h"
#include "sys_cfg.h"
#include "sys_ctrl.h"

extern "C" {

system_info_t system_info;

extern SPI_HandleTypeDef hspi1;

uint8_t _start_boot_share_data_flash[sizeof(sys_boot_t)] = {0};

static uint32_t soft_watchdog_limit_ticks = 0;
static uint32_t soft_watchdog_counter_ticks = 0;
static uint8_t soft_watchdog_enabled = 0;
extern uint32_t _etext;

}

extern "C" {

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

uint8_t flash_transfer(uint8_t data)
{
	uint8_t rx = 0U;
	(void)HAL_SPI_TransmitReceive(&hspi1, &data, &rx, 1U, HAL_MAX_DELAY);
	return rx;
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
	sys_cfg_update_info();
}

void sys_ctrl_reset(void)
{
	NVIC_SystemReset();
}

void sys_ctrl_independent_watchdog_init(void)
{
	#if (SYS_WATCHDOG_MODE == SYS_WATCHDOG_MODE_IWDG)
	__HAL_RCC_LSI_ENABLE();
	while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET) {
	}

	#if (SYS_IWDG_FREEZE_IN_DEBUG != 0U)
	__HAL_DBGMCU_FREEZE_IWDG();
	#endif

	IWDG->KR = 0x5555U;
	IWDG->PR = 0x06U;
	IWDG->RLR = 4095U;
	while (IWDG->SR != 0U) {
	}
	IWDG->KR = 0xCCCCU;
	IWDG->KR = 0xAAAAU;
	#else
	/* watchdog is disabled or handled by another source in this build */
	#endif
}

void sys_ctrl_independent_watchdog_reset(void)
{
	#if (SYS_WATCHDOG_MODE == SYS_WATCHDOG_MODE_IWDG)
	IWDG->KR = 0xAAAAU;
	#else
	/* keep API compatibility when IWDG is not selected */
	#endif
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

} // extern "C"
