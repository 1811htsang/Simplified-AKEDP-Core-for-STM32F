#ifndef __FLASH_H__
  #define __FLASH_H__

  // Khai báo directive C cho C++
  #ifdef __cplusplus
    extern "C"
    {
  #endif

    // Khai báo thư viện sử dụng
    #include <stdint.h>

    // Định nghĩa các hằng số liên quan đến flash
    #define FLASH_PAGE_SIZE 0x400U          // Kích thước trang flash (1KB)
    #define FLASH_BLOCK_64K_SIZE 0x10000U   // Kích thước block 64KB

    // Khai báo các hàm liên quan đến flash
    void flash_read(uint32_t address, uint8_t* data, uint32_t length);  // Hàm đọc dữ liệu từ flash
    void flash_write(uint32_t address, uint8_t* data, uint32_t length); // Hàm ghi dữ liệu vào flash
    void flash_erase_sector(uint32_t address);                          // Hàm xóa sector trong flash
    void flash_erase_block_64k(uint32_t address);                       // Hàm xóa block 64KB trong flash

    // Các hàm nội bộ để quản lý flash
    void internal_flash_unlock(void);                                                 // Hàm mở khóa flash để cho phép ghi/xóa
    void internal_flash_lock(void);                                                   // Hàm khóa flash sau khi hoàn thành ghi/xóa
    void internal_flash_erase_pages_cal(uint32_t address, uint32_t length);           // Hàm xóa các trang flash dựa trên địa chỉ và độ dài
    void internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t length);  // Hàm ghi dữ liệu vào flash dựa trên địa chỉ và độ dài

  #ifdef __cplusplus
  }
  #endif

#endif