#ifndef __APP_FLASH_H__
  #define __APP_FLASH_H__

  // Khai báo các địa chỉ sector flash để lưu trữ log và dump RAM
  #define APP_FLASH_AK_DBG_FATAL_LOG_SECTOR						(0x2000)  // Sector để lưu log lỗi nghiêm trọng
  #define APP_FLASH_AK_DBG_IRQ_LOG_SECTOR							(0x3000)  // Sector để lưu log ngắt
  #define APP_FLASH_AK_DBG_MSG_SECTOR_1								(0x4000)  // Sector để lưu log thông thường
  #define APP_FLASH_AK_DBG_MSG_SECTOR_2								(0x5000)  // Sector để lưu log thông thường
  #define APP_FLASH_DUMP_RAM_START_ADDR								(0x20000) // Địa chỉ bắt đầu của RAM để dump

#endif //__APP_FLASH_H__
