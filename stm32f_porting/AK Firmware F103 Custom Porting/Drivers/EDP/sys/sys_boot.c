// Khai báo thư viện sử dụng
#include <string.h>
#include "sys_boot.h"
#include "flash.h"

// Định nghĩa biến toàn cục
static sys_boot_t sys_boot_obj;

// Khai báo các pragma 
#pragma GCC diagnostic push
#pragma GCC diagnostic pop
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow="

// Hàm khởi tạo hệ thống boot, sao chép dữ liệu từ flash vào RAM
void sys_boot_init() {
	extern uint8_t _start_boot_share_data_flash[];
	memcpy((uint8_t*)&sys_boot_obj, (uint8_t*)_start_boot_share_data_flash, sizeof(sys_boot_t));
}

// Hàm lấy thông tin hệ thống boot, sao chép dữ liệu từ RAM vào đối tượng đầu vào
void sys_boot_get(sys_boot_t* obj) {
	memcpy((uint8_t*)obj, (uint8_t*)&sys_boot_obj, sizeof(sys_boot_t));
}

// Hàm cập nhật thông tin hệ thống boot, sao chép dữ liệu từ đối tượng đầu vào vào RAM và flash
uint8_t sys_boot_set(sys_boot_t* sys_boot) {
	extern uint8_t _start_boot_share_data_flash[];

	/* update RAM object */
	memcpy((uint8_t*)&sys_boot_obj, (uint8_t*)sys_boot, sizeof(sys_boot_t));

	/* update internal flash */
	internal_flash_unlock();
	internal_flash_erase_pages_cal((uint32_t)_start_boot_share_data_flash, sizeof(sys_boot_t));
	internal_flash_write_cal((uint32_t)_start_boot_share_data_flash, (uint8_t*)sys_boot, sizeof(sys_boot_t));

	return SYS_BOOT_OK;
}

