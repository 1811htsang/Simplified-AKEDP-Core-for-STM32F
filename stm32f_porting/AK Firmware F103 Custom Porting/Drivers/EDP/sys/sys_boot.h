#ifndef __SYS_BOOT_H__
	#define __SYS_BOOT_H__

	// Khai báo directive C cho C++
	#ifdef __cplusplus
	extern "C"
	{
	#endif

		// Khai báo thư viện sử dụng
		#include <stdint.h>
		#include "port.h"

		// Khai báo kiểu trả về của hàm
		#define SYS_BOOT_OK							0x00
		#define SYS_BOOT_NG							0x01

		// Khai báo magic number để xác thực firmware
		#define FIRMWARE_PSK						0x18112005 /* magic number */

		// Khai báo cấu trúc dữ liệu chứa thông tin về firmware
		typedef struct {
			uint32_t psk; 			// magic number để xác thực firmware
			uint32_t bin_len;		// độ dài của firmware binary
			uint16_t checksum;	// checksum của firmware binary
		} firmware_header_t;

		// Khai báo cấu trúc dữ liệu phản hồi msg của host
		typedef struct {
			uint8_t type; 				// Loại tin nhắn
			uint8_t src_task_id; 	// ID của task gửi tin nhắn
			uint8_t des_task_id; 	// ID của task nhận tin nhắn
			uint8_t sig; 					// Tín hiệu của tin nhắn
			uint8_t if_src_type; 	// Loại nguồn gửi tin nhắn
			uint8_t if_des_type; 	// Loại đích nhận tin nhắn
		} __AK_PACKETED ak_msg_host_res_t;

		// Khai báo cấu trúc dữ liệu chứa thông tin về hệ thống boot
		typedef struct {
			// Thông tin về firmware hiện tại đang chạy trên thiết bị
			firmware_header_t current_fw_boot_header;
			firmware_header_t current_fw_app_header;

			// Thông tin về firmware mới được cập nhật
			firmware_header_t update_fw_boot_header;
			firmware_header_t update_fw_app_header;
		} sys_boot_t;

		// Khai báo các hàm liên quan đến hệ thống boot
		extern void sys_boot_init(); 							// Hàm khởi tạo hệ thống boot, sao chép dữ liệu từ flash vào RAM
		extern void sys_boot_get(sys_boot_t*); 		// Hàm lấy thông tin hệ thống boot, sao chép dữ liệu từ RAM vào đối tượng đầu vào
		extern uint8_t sys_boot_set(sys_boot_t*); // Hàm cập nhật thông tin hệ thống boot, sao chép dữ liệu từ đối tượng đầu vào vào RAM và flash

	#ifdef __cplusplus
	}
	#endif

#endif //__SYS_BOOT_H__

