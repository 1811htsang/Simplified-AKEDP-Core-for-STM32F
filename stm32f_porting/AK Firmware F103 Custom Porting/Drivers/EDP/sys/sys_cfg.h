#ifndef __SYS_CFG_H__
  #define __SYS_CFG_H__

  // Khai báo directive C cho C++
  #ifdef __cplusplus
  extern "C"
  {
  #endif

    // Khai báo thư viện sử dụng
    #include <stdint.h>
    #include "main.h"
    #include "system.h"

    // Khai báo các định nghĩa về chế độ watchdog
    #define SYS_WATCHDOG_MODE_NONE 0U // Không sử dụng watchdog
    #define SYS_WATCHDOG_MODE_IWDG 1U // Sử dụng Independent Watchdog (IWDG)
    #define SYS_WATCHDOG_MODE_SOFT 2U // Sử dụng watchdog phần mềm (soft watchdog)

    // Khai báo chế độ watchdog
    #define SYS_WATCHDOG_MODE SYS_WATCHDOG_MODE_NONE // Mặc định không sử dụng watchdog
    #define SYS_IWDG_FREEZE_IN_DEBUG 1U // Cho phép đóng băng IWDG khi debug

    // Khai báo các hàm cấu hình hệ thống
    void sys_cfg_init(void);        // Hàm khởi tạo cấu hình hệ thống
    void sys_cfg_clock(void);       // Hàm cấu hình hệ thống clock
    void sys_cfg_tick(void);        // Hàm cấu hình hệ thống tick (systick)
    void sys_cfg_update_info(void); // Hàm cập nhật thông tin hệ thống 

  #ifdef __cplusplus
  }
  #endif

#endif
