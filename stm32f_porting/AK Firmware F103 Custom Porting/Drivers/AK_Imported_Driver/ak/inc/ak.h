#ifndef __AK_H__
  #define __AK_H__

  // Khai báo C directive cho C++ 
  #ifdef __cplusplus
  extern "C"
  {
  #endif

    // Khai báo các thư viện sử dụng
    #include <string.h>
    #include "fsm.h"  // Thư viện FSM (Finite State Machine) để quản lý trạng thái của hệ thống
    #include "task.h" // Thư viện quản lý tác vụ, giúp tạo và quản lý các tác vụ trong hệ thống
    #include "port.h" // Thư viện quản lý attribute và dynamic allocation

    // Khai báo phiên bản AK-EDP
    #define AK_VERSION						"1.0"

    // Khai báo hằng số chung
    #define AK_ENABLE						(0x01u)
    #define AK_DISABLE					(0x00u)
    #define AK_FLAG_ON					(0x01u)
    #define AK_FLAG_OFF					(0x00u)
    #define AK_RET_OK						(0x01u)
    #define AK_RET_NG						(0x00u)

    // Khai báo hằng số cho tín hiệu người dùng
    #define AK_USER_DEFINE_SIG				(10)

    // Khai báo hằng số cho mức ưu tiên của tác vụ
    #define TASK_PRI_MAX_SIZE				(8)
    #define TASK_PRI_LEVEL_0				(0) 
    #define TASK_PRI_LEVEL_1				(1)
    #define TASK_PRI_LEVEL_2				(2)
    #define TASK_PRI_LEVEL_3				(3)
    #define TASK_PRI_LEVEL_4				(4)
    #define TASK_PRI_LEVEL_5				(5)
    #define TASK_PRI_LEVEL_6				(6)
    #define TASK_PRI_LEVEL_7				(7)

    // Khai báo hằng số cho ID của tác vụ
    #define AK_TASK_INTERRUPT_ID		(0xEE) // Quản lý ID của tác vụ ngắt, giúp xác định và xử lý các tác vụ liên quan đến ngắt trong hệ thống
    #define AK_TASK_IDLE_ID					(0xEF) // Quản lý ID của tác vụ nhàn rỗi, giúp xác định và xử lý tác vụ nhàn rỗi trong hệ thống

  #ifdef __cplusplus
  }
  #endif

#endif // __AK_H__
