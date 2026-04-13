// Khai báo thư viện sử dụng
#include <cstring>
#include "main.h"
#include "app.h"
#include "flash.h"
#include "platform.h"
#include "sys_boot.h"
#include "sys_cfg.h"
#include "sys_ctrl.h"

// Khai báo directive C cho C++
extern "C" {
	// Biến toàn cục chứa thông tin hệ thống
	system_info_t system_info;

	// Khai báo biến toàn cục cho SPI handle
	extern SPI_HandleTypeDef hspi1;

	// Biến toàn cục chứa dữ liệu chia sẻ giữa bootloader và ứng dụng, được đặt ở một vị trí flash cụ thể
	uint8_t _start_boot_share_data_flash[sizeof(sys_boot_t)] = {0};

	// Biến toàn cục cho soft watchdog
	static uint32_t soft_watchdog_limit_ticks = 0; 		// Số tick giới hạn trước khi soft watchdog kích hoạt reset
	static uint32_t soft_watchdog_counter_ticks = 0;	// Bộ đếm tick hiện tại của soft watchdog
	static uint8_t soft_watchdog_enabled = 0;					// Cờ cho biết soft watchdog có đang được kích hoạt hay không
	extern uint32_t _etext;														// Biến toàn cục chứa địa chỉ kết thúc của phần text (code) trong flash
}

// Hàm nội bộ để kích hoạt bộ đếm chu kỳ DWT, cho phép đo thời gian chính xác
static void enable_dwt_cycle_counter(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0U;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

// Hàm nội bộ để lấy kích thước flash của vi điều khiển
static uint32_t get_flash_length_bytes(void)
{
	return 64U * 1024U;
}

// Hàm nội bộ để chuẩn hóa địa chỉ flash, đảm bảo rằng địa chỉ nằm trong phạm vi flash
static uint32_t normalize_flash_address(uint32_t address)
{
	if (address < FLASH_BASE) {
		return FLASH_BASE + address;
	}
	return address;
}

// Hàm nội bộ để kiểm tra xem một địa chỉ có nằm trong phạm vi flash hay không
static uint8_t is_internal_flash_address(uint32_t address)
{
	uint32_t normalized = normalize_flash_address(address);
	return (normalized >= FLASH_BASE) && (normalized < (FLASH_BASE + get_flash_length_bytes()));
}

// Hàm nội bộ để ghi dữ liệu vào flash, sử dụng HAL_FLASH_Program để ghi từng half-word (16 bit) một cách an toàn
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

// Hàm nội bộ để xóa các trang flash dựa trên địa chỉ và độ dài, sử dụng HAL_FLASHEx_Erase với cấu hình phù hợp
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

// Hàm để nhập và thoát critical section, sử dụng __disable_irq và __enable_irq để quản lý ngắt
void platform_enter_critical(void)
{
	__disable_irq();
}

// Hàm để thoát critical section, sử dụng __enable_irq để cho phép ngắt trở lại
void platform_exit_critical(void)
{
	__enable_irq();
}

// Hàm để truyền và nhận dữ liệu qua SPI, sử dụng HAL_SPI_TransmitReceive để gửi một byte và nhận một byte đồng thời
uint8_t flash_transfer(uint8_t data)
{
	uint8_t rx = 0U;
	(void)HAL_SPI_TransmitReceive(&hspi1, &data, &rx, 1U, HAL_MAX_DELAY);
	return rx;
}

// Hàm để mở khóa flash, sử dụng HAL_FLASH_Unlock để cho phép ghi và xóa flash
void internal_flash_unlock(void)
{
	HAL_FLASH_Unlock();
}

// Hàm để khóa flash, sử dụng HAL_FLASH_Lock để ngăn chặn ghi và xóa flash
void internal_flash_lock(void)
{
	HAL_FLASH_Lock();
}

// Hàm để xóa các trang flash dựa trên địa chỉ và độ dài, gọi hàm nội bộ flash_erase_pages để thực hiện xóa
void internal_flash_erase_pages_cal(uint32_t address, uint32_t length)
{
	flash_erase_pages(address, length);
}

// Hàm để ghi dữ liệu vào flash dựa trên địa chỉ và độ dài, gọi hàm nội bộ flash_program_bytes để thực hiện ghi
void internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t length)
{
	flash_program_bytes(address, data, length);
}

// Hàm để đọc dữ liệu từ flash, gọi hàm nội bộ flash_read để thực hiện đọc
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

// Hàm để ghi dữ liệu vào flash, gọi hàm nội bộ flash_program_bytes để thực hiện ghi
void flash_write(uint32_t address, uint8_t* data, uint32_t length)
{
	flash_program_bytes(address, data, length);
}

// Hàm để xóa một sector trong flash, gọi hàm nội bộ flash_erase_pages với kích thước trang để thực hiện xóa
void flash_erase_sector(uint32_t address)
{
	flash_erase_pages(address, FLASH_PAGE_SIZE);
}

// Hàm để xóa một block 64KB trong flash, gọi hàm nội bộ flash_erase_pages với kích thước block để thực hiện xóa
void flash_erase_block_64k(uint32_t address)
{
	flash_erase_pages(address, FLASH_BLOCK_64K_SIZE);
}

// Hàm để cấu hình clock của hệ thống, thiết lập nhóm ưu tiên ngắt và kích hoạt bộ đếm chu kỳ DWT để đo thời gian chính xác
void sys_cfg_clock(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	enable_dwt_cycle_counter();
	system_info.cpu_clock = HAL_RCC_GetHCLKFreq();
}

// Hàm để cấu hình tick của hệ thống, thiết lập ưu tiên ngắt cho SysTick để đảm bảo rằng tick có độ ưu tiên thấp nhất
void sys_cfg_tick(void)
{
	HAL_NVIC_SetPriority(SysTick_IRQn, 15U, 0U);
}

// Hàm để cập nhật thông tin hệ thống, tính toán các giá trị như flash đã sử dụng, RAM đã sử dụng, kích thước dữ liệu đã khởi tạo, kích thước dữ liệu chưa khởi tạo, bộ nhớ stack còn lại, bộ nhớ heap còn lại và các thông tin khác dựa trên các biểu tượng linker script
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

// Hàm để khởi tạo cấu hình hệ thống, gọi các hàm cấu hình clock, tick và cập nhật thông tin hệ thống
void sys_cfg_init(void)
{
	sys_cfg_clock();
	sys_cfg_tick();
	sys_cfg_update_info();
}

// Hàm để reset hệ thống, sử dụng NVIC_SystemReset để khởi động lại vi điều khiển
void sys_ctrl_reset(void)
{
	NVIC_SystemReset();
}

// Hàm để khởi tạo watchdog độc lập, cấu hình IWDG nếu được chọn và thiết lập thời gian timeout
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
		// HWDG tự động quản lý thay cho IWDG
	#endif
}

// Hàm để reset watchdog độc lập, gọi lệnh reset của IWDG nếu được chọn để ngăn chặn watchdog kích hoạt reset hệ thống
void sys_ctrl_independent_watchdog_reset(void)
{
	#if (SYS_WATCHDOG_MODE == SYS_WATCHDOG_MODE_IWDG)
	IWDG->KR = 0xAAAAU;
	#else
	/* keep API compatibility when IWDG is not selected */
	#endif
}

// Hàm để khởi tạo soft watchdog, thiết lập thời gian timeout và kích hoạt soft watchdog
void sys_ctrl_soft_watchdog_init(uint32_t timeout_ticks)
{
	soft_watchdog_limit_ticks = timeout_ticks;
	soft_watchdog_counter_ticks = 0U;
	soft_watchdog_enabled = 1U;
}

// Hàm để reset soft watchdog, đặt lại bộ đếm tick của soft watchdog về 0
void sys_ctrl_soft_watchdog_reset(void)
{
	soft_watchdog_counter_ticks = 0U;
}

// Hàm để kích hoạt soft watchdog, đặt cờ enabled của soft watchdog thành 1 để cho phép soft watchdog hoạt động
void sys_ctrl_soft_watchdog_enable(void)
{
	soft_watchdog_enabled = 1U;
}

// Hàm để vô hiệu hóa soft watchdog, đặt cờ enabled của soft watchdog thành 0 để ngăn chặn soft watchdog hoạt động
void sys_ctrl_soft_watchdog_disable(void)
{
	soft_watchdog_enabled = 0U;
}

// Hàm để tăng bộ đếm của soft watchdog, được gọi định kỳ để cập nhật bộ đếm 
void sys_ctrl_soft_watchdog_increase_counter(void)
{
	if ((soft_watchdog_enabled != 0U) && (soft_watchdog_limit_ticks != 0U)) {
		soft_watchdog_counter_ticks++;
		if (soft_watchdog_counter_ticks >= soft_watchdog_limit_ticks) {
			sys_ctrl_reset();
		}
	}
}

// Hàm để tạo độ trễ bằng cách sử dụng vòng lặp đếm, sử dụng __NOP để đảm bảo rằng vòng lặp không bị tối ưu hóa hoàn toàn
void sys_ctrl_delay(volatile uint32_t count)
{
	while (count-- != 0U) {
		__NOP();
	}
}

// Hàm để tạo độ trễ bằng cách sử dụng HAL_Delay, cho phép tạo độ trễ chính xác dựa trên tick của hệ thống
void sys_ctrl_delay_ms(volatile uint32_t count)
{
	HAL_Delay(count);
}

// Hàm để tạo độ trễ bằng cách sử dụng bộ đếm chu kỳ DWT, cho phép tạo độ trễ chính xác dựa trên số chu kỳ của CPU
void sys_ctrl_delay_us(volatile uint32_t count)
{
	uint32_t start = DWT->CYCCNT;
	uint32_t cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
	uint32_t target = count * cycles_per_us;
	while ((DWT->CYCCNT - start) < target) {
	}
}

// Hàm để lấy thời gian hiện tại tính bằng microsecond, sử dụng bộ đếm chu kỳ DWT để tính toán thời gian dựa trên số chu kỳ đã trôi qua
uint32_t sys_ctrl_micros(void)
{
	uint32_t cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
	if (cycles_per_us == 0U) {
		return 0U;
	}
	return DWT->CYCCNT / cycles_per_us;
}

// Hàm để lấy thời gian hiện tại tính bằng millisecond, sử dụng HAL_GetTick để trả về số tick đã trôi qua kể từ khi hệ thống khởi động
uint32_t sys_ctrl_millis(void)
{
	return HAL_GetTick();
}

// Hàm để lấy thông tin firmware, điền vào cấu trúc firmware_header_t với các giá trị như psk, độ dài binary và checksum
void sys_ctrl_get_firmware_info(firmware_header_t* firmware_header)
{
	if (firmware_header == NULL) {
		return;
	}

	firmware_header->psk = FIRMWARE_PSK;
	firmware_header->bin_len = (uint32_t)&_etext - FLASH_BASE;
	firmware_header->checksum = 0U;
}

// Hàm để vào chế độ chờ, sử dụng lệnh __WFI để đưa vi điều khiển vào chế độ chờ cho đến khi có ngắt xảy ra
void sys_ctrl_sleep_wait_for_irq(void)
{
	__WFI();
}

// Hàm để lấy số ngoại lệ hiện tại, sử dụng hàm __get_IPSR để trả về số ngoại lệ đang được xử lý
uint32_t sys_ctrl_get_exception_number(void)
{
	return __get_IPSR();
}

// Hàm để khởi động lại ứng dụng, gọi hàm reset của hệ thống để khởi động lại vi điều khiển
void sys_ctrl_restart_app(void)
{
	sys_ctrl_reset();
}

// Hàm để dừng MCU, sử dụng HAL_PWR_EnterSTANDBYMode để đưa vi điều khiển vào chế độ standby, giảm thiểu tiêu thụ năng lượng
void sys_ctrl_stop_mcu(void)
{
	HAL_PWR_EnterSTANDBYMode();
}

// Hàm kiểm tra xem hệ thống có đang chạy trong bootloader hay không, dựa trên giá trị của biểu tượng linker script để xác định
int asm_test_add(int lhs, int rhs)
{
	return lhs + rhs;
}

// Hàm để lấy kích thước stack đã sử dụng, tính toán dựa trên địa chỉ của _estack và vị trí hiện tại của stack pointer
uint32_t sys_stack_count_init(void)
{
	return sys_stack_get_size();
}

// Hàm để lấy kích thước stack còn lại, tính toán dựa trên địa chỉ của _estack và vị trí hiện tại của stack pointer
uint32_t sys_stack_usage(void)
{
	extern uint32_t _estack;
	uint32_t current_sp = __get_MSP();
	return (uint32_t)&_estack - current_sp;
}

// Hàm để lấy tổng kích thước stack, tính toán dựa trên địa chỉ của _estack và _ebss để xác định kích thước stack có sẵn
uint32_t sys_stack_get_size(void)
{
	extern uint32_t _estack;
	extern uint32_t _ebss;
	return (uint32_t)&_estack - (uint32_t)&_ebss;
}

