# Hướng dẫn sử dụng AKEDP Core

**Phiên bản**: 1.0  
**Tác giả**: Shang Huang  
**Ngày cập nhật**: 09/04/2026

---

## Ghi chú

Hiện tại đã kiểm tra và sửa lỗi để cập nhật tài liệu khớp với mã nguồn EDP. Nếu có phát hiện các lỗi sai nào, xin hãy tạo issue ticket trên Github.

---

## Mục lục

1. [Giới thiệu chung](#giới-thiệu-chung)
2. [Kiến trúc EDP Core](#kiến-trúc-edp-core)
3. [Quản lý tác vụ (Task Management)](#quản-lý-tác-vụ-task-management)
4. [Hệ thống tin nhắn (Message System)](#hệ-thống-tin-nhắn-message-system)
5. [Quản lý bộ hẹn giờ (Timer Management)](#quản-lý-bộ-hẹn-giờ-timer-management)
6. [Máy trạng thái hữu hạn (FSM)](#máy-trạng-thái-hữu-hạn-fsm)
7. [Lớp trừu tượng nền tảng (Platform Abstraction)](#lớp-trừu-tượng-nền-tảng-platform-abstraction)
8. [Quản lý ngắt (Interrupt Management)](#quản-lý-ngắt-interrupt-management)
9. [Cấu trúc dữ liệu tiện ích (Utility Data Structures)](#cấu-trúc-dữ-liệu-tiện-ích-utility-data-structures)
10. [Ví dụ thực tế (Practical Examples)](#ví-dụ-thực-tế-practical-examples)

---

## Giới thiệu chung

### AKEDP Core

**AKEDP (Active Kernel Embedded Development Platform)** là một nền tảng phát triển nhúng được thiết kế cho các hệ thống nhúng thời gian thực. AKEDP cung cấp một kiến trúc phần mềm hoàn chỉnh bao gồm:

- **Hạt nhân**: Bộ lập lịch tác vụ (Task Scheduler), quản lý tin nhắn, và bộ hẹn giờ
- **Tầng ứng dụng (Application Layer)**: Các tác vụ ứng dụng
- **Tầng hệ thống (System Layer)**: Trừu tượng hóa nền tảng, quản lý ngắt, I/O điều khiển
- **Thư viện công cụ (Utility Libraries)**: Cấu trúc dữ liệu (FIFO, Ring Buffer), tiện ích thao tác bộ nhớ

Ngoài ra **EDP** còn đại diện cho **Event-Driven Programming**, một mô hình lập trình trong đó luồng điều khiển được xác định bởi các sự kiện (events) như tin nhắn, ngắt, hoặc bộ hẹn giờ.

### Đặc điểm chính

| Đặc điểm | Mô tả |
| ---------- | ------- |
| **Tác vụ đa ưu tiên** | 8 mức ưu tiên (0-7), hỗ trợ tác vụ idle |
| **Giao tiếp tin nhắn** | Hệ thống tin nhắn bất đồng bộ với 3 loại pool (common, pure, dynamic) |
| **Quản lý thời gian** | Bộ hẹn giờ với hỗ trợ one-shot và periodic |
| **Trừu tượng hóa phần cứng** | Lớp platform abstraction có thể tái sử dụng trên các MCU khác cùng core |
| **Hỗ trợ FSM** | Máy trạng thái hữu hạn để quản lý logic phức tạp |
| **An toàn luồng** | Cực tiểu hóa các phần tới hạn (critical sections) để tăng hiệu suất |

---

## Kiến trúc EDP Core

### Sơ đồ kiến trúc

```structure
┌─────────────────────────────────────────────────────────┐
│            Application Layer (app/)                     │
│                 (Interface Task)                        │
├─────────────────────────────────────────────────────────┤
│            AKEDP Core (ak/)                             │
│  ┌──────────────┬──────────────┬─────────┬─────┬──────┐ │
│  │   Task Mgmt  │ Message Mgmt │   Timer | FSM | TSM  │ |
│  └──────────────┴──────────────┴─────────┴─────┴──────┘ │
├─────────────────────────────────────────────────────────┤
│         System & Platform Layer (sys/)                  │
│  ┌──────────────┬──────────────┬──────────────────────┐ │
│  │ Fatal Debug  │  Platform    │   I/O & Device       │ │
│  │ Management   │  Abstraction │   Drivers (UART,LED) │ │
│  └──────────────┴──────────────┴──────────────────────┘ │
├─────────────────────────────────────────────────────────┤
│        Utility Libraries (common/)                      │
│  ┌────────────────────────┬───────────────────────────┐ │
│  │   Containers           │               Utils       │ │
│  │  (FIFO, RB)            |                           │ │
│  └────────────────────────┴───────────────────────────┘ │
├─────────────────────────────────────────────────────────┤
│         STM32 HAL & Hardware Layer                      │
│  (stm32f1xx_hal.h, CMSIS, Device-specific)              │
└─────────────────────────────────────────────────────────┘
```

### Cấu trúc thư mục

```file_structure
EDP/
├── core/                         # Hạt nhân AKEDP
│   ├── inc/                     # Header files chính
│   │   ├── ak.h                 # Định nghĩa cơ bản khung AKEDP
│   │   ├── task.h               # Quản lý tác vụ
│   │   ├── message.h            # Hệ thống tin nhắn
│   │   ├── timer.h              # Quản lý bộ hẹn giờ
│   │   ├── fsm.h                # Máy trạng thái hữu hạn
│   │   ├── port.h               # Cấu hình và cấp phát động
│   │   └── tsm.h                # Máy trạng thái tác vụ 
│   ├── src/                     # Triển khai
│   │   ├── task.c
│   │   ├── message.c
│   │   ├── timer.c
│   │   └── fsm.c
│   ├── doc/                     # Tài liệu tham chiếu xây dựng core
│   └── ak.cfg.mk                # Cấu hình xây dựng
│
├── app/                         # Tầng ứng dụng (Nằm trong diện xem xét mở rộng chức năng)
│   ├── app.h/cpp                # Định nghĩa toàn bộ chương trình hoạt động với EDP
│   ├── task_if.h/cpp            # Tác vụ giao thức để truyền message từ các interface vào hệ thống
│   └── task_list.h/cpp          # Quản lý danh sách tác vụ
│
├── sys/                         # Tầng hệ thống (Nằm trong diện xem xét sửa đổi để core trở nên độc lập khỏi phần cứng cụ thể)
│   ├── platform.h               # Điều khiển ngắt, trừu tượng nền tảng
│   ├── system.h                 # Thông tin hệ thống
│   ├── sys_cfg.h                # Cấu hình hệ thống (Nằm trong diện xem xét loại bỏ)
│   ├── sys_dbg.h/c              # Tiện ích debug 
│   ├── porting_compat.h         # Cấu hình chuyển đổi tương thích từ L151 sang F103
│   └── sys_ctrl.h               # Điều khiển hệ thống
│
└── common/                      # Thư viện công cụ
    ├── container/
    │   ├── fifo.h/c             # Hàng đợi FIFO
    │   ├── ring_buffer.h/c      # Bộ đệm vòng
    │   └── log_queue.h/c        # Hàng đợi ghi nhật ký
    └── utils.h/c                # Các hàm tiện ích
```

---

## Quản lý tác vụ (Task Management)

### Khái niệm cơ bản

Một **tác vụ (task)** là một đơn vị công việc độc lập có thể được lập lịch bởi bộ lập lịch của hạt nhân AKEDP. AKEDP hỗ trợ hai loại tác vụ:

1. **Message-driven tasks**: Tác vụ được kích hoạt bởi tin nhắn
2. **Polling tasks**: Tác vụ được gọi định kỳ

### Định nghĩa kiểu dữ liệu

```c
// task.h
typedef uint8_t task_id_t;                    // ID tác vụ
typedef uint8_t task_pri_t;                   // Mức ưu tiên (0-7)
typedef void (*pf_task)(ak_msg_t*);           // Hàm xử lý tác vụ (message-driven)
typedef void (*pf_task_polling)();            // Hàm xử lý tác vụ (polling)

// Cấu trúc tác vụ
typedef struct task_t {
  task_id_t id;                             // ID tác vụ
  task_pri_t pri;                           // Ưu tiên (0=cao nhất, 7=thấp nhất)
  pf_task task;                             // Hàm xử lý
} task_t;

typedef struct task_polling_t {
  task_id_t id;
  uint8_t ability;                          // Khả năng: 1=enabled, 0=disabled
  pf_task_polling task_polling;
} task_polling_t;
```

### ID tác vụ tiêu chuẩn

```c
#define AK_TASK_IDLE_ID (0xEF) // Quản lý ID của tác vụ nhàn rỗi, giúp xác định và xử lý tác vụ nhàn rỗi trong hệ thống

// Các ID tác vụ khác có thể được định nghĩa tùy theo ứng dụng
```

### Tạo một tác vụ mới

#### Ví dụ 1: Tác vụ xử lý tin nhắn (Message-driven)

```c
// header file: task_my_app.h
#ifndef TASK_MY_APP_H
  #define TASK_MY_APP_H

  // Khai báo thư viện hạt nhân
  #include "ak.h"

  // Định nghĩa ID tác vụ phục vụ ứng dụng
  #define AC_MY_APP_TASK_ID           10

  // Định nghĩa các tín hiệu ứng dụng
  #define AC_MY_APP_INIT              1
  #define AC_MY_APP_PROCESS_DATA      2
  #define AC_MY_APP_ERROR             3

  // Khai báo hàm xử lý tác vụ
  void task_my_app_handler(ak_msg_t* msg);
#endif // TASK_MY_APP_H
```

```c
// source file: task_my_app.c
#include "task_my_app.h"

void task_my_app_handler(ak_msg_t* msg) {
  switch (msg->sig) { // Phụ thuộc vào tín hiệu (signal) của tin nhắn để xác định hành động cần thực hiện
    case AC_MY_APP_INIT: {
      // Khởi tạo tác vụ
      printf("My App Task Initialized\n");
      break;
    }
    
    case AC_MY_APP_PROCESS_DATA: {
      // Xử lý dữ liệu từ sender
      uint8_t* data = (uint8_t*)msg->data;
      printf("Received data: %d\n", *data);
      break;
    }
    
    case AC_MY_APP_ERROR: {
      // Xử lý lỗi
      printf("Error occurred\n");
      break;
    }
    
    default:
      printf("Unknown signal: %d\n", msg->sig);
      break;
  }
}
```

#### Ví dụ 2: Tác vụ polling định kỳ

```c
// header file: task_polling_example.h
#ifndef TASK_POLLING_EXAMPLE_H
  #define TASK_POLLING_EXAMPLE_H

  #define AC_POLLING_TASK_ID          11

  void task_polling_example();
#endif // TASK_POLLING_EXAMPLE_H
```

```c
// source file: task_polling_example.c
#include "task_polling_example.h"
#include "platform.h"

void task_polling_example() {
  static uint32_t counter = 0;
  
  counter++;
  
  // Thực hiện công việc định kỳ
  if (counter % 10 == 0) {
    printf("Polling task running: %lu\n", counter);
  }
  
  // Có thể gửi tin nhắn nếu cần
  if (counter >= 100) {
    ak_msg_t* msg = ak_alloc_msg(AC_MY_APP_TASK_ID, AC_MY_APP_PROCESS_DATA);
    if (msg) {
      *((uint8_t*)msg->data) = 42;
      ak_post_msg(msg);
    }
    counter = 0;
  }
}
```

### Đăng ký tác vụ

```c
// header file: header.h
#ifndef HEADER_H
  #define HEADER_H

  #include "ak.h"
  #include "message.h"
  #include "task.h"
  #include "task_list.h"

  // Định nghĩa ID tác vụ
  #define Ak_TASK_A_ID          11

  // Định nghĩa tín hiệu
  #define SIG_A_INIT (0xA0u)

  // Khai báo bảng lưu task
  extern task_t task_table[];

  void task_a_handler(ak_msg_t* msg);
#endif // HEADER_H
```

```c
#include "header.h"

// Định nghĩa bảng lưu task
task_t task_table[] = {
  {AC_MY_APP_TASK_ID, 3, task_my_app_handler},
  {AK_TASK_EOT_ID, TASK_PRI_LEVEL_0, (pf_task)0} // Báo End-Of-Table
};

void task_a_handler(ak_msg_t* msg) {
  // Xử lý tin nhắn cho Task A
}

// Trong hàm main

// Khai báo thông số bảng task cho AKEDP core
task_create((task_t*)task_table);

// Post tin nhắn khởi tạo cho Task A
task_post_common_msg(TASK_A_ID, SIG_A_INIT, NULL, 0);
```

Lưu ý: Đối với việc khởi tạo chương trình hoạt động mà không dùng tới phần app trong core AKEDP thì có thể khởi tạo bảng task_table theo nhu cầu sử dụng. Đối với chương trình sử dụng phần app trong core AKEDP thì nên ưu tiên sử dụng `app_task_table` để đảm bảo tính tương thích và dễ dàng tích hợp với các phần khác của hệ thống.

---

## Hệ thống tin nhắn (Message System)

### Khái niệm tin nhắn

Hệ thống tin nhắn là cơ chế truyền thông chính giữa các tác vụ trong EDP. Các tin nhắn được gửi không đồng bộ, cho phép tác vụ sẽ xử lý chúng theo thứ tự ưu tiên.

### Cấu trúc tin nhắn

```c
// message.h
typedef struct ak_msg_t {
  // Quản lý liên kết tin nhắn trong pool và queue
  struct ak_msg_t*  next; // Quản lý con trỏ đến tin nhắn tiếp theo trong pool hoặc queue, giúp xây dựng danh sách liên kết cho việc quản lý bộ nhớ và hàng đợi tin nhắn

  // Quản lý thông tin cơ bản của tin nhắn, bao gồm ID của tác vụ nguồn và đích, số lượng tham chiếu, tín hiệu, và thông tin giao diện bên ngoài
  uint8_t src_task_id;  // Quản lý ID của tác vụ nguồn gởi tin nhắn
  uint8_t des_task_id;  // Quản lý ID của tác vụ đích nhận tin nhắn
  uint8_t ref_count;  // Quản lý số lượng tham chiếu đến tin nhắn, giúp quản lý bộ nhớ hiệu quả
  uint8_t sig;  // Quản lý tín hiệu của tin nhắn, giúp xác định loại tin nhắn và cách xử lý nó
  uint8_t if_src_task_id; // Quản lý ID của tác vụ nguồn gởi tin nhắn qua giao diện bên ngoài
  uint8_t if_des_task_id; // Quản lý ID của tác vụ đích nhận tin nhắn qua giao diện bên ngoài
  uint8_t if_src_type; // Quản lý loại nguồn của tin nhắn qua giao diện bên ngoài, giúp xác định nguồn gốc của tin nhắn
  uint8_t if_des_type; // Quản lý loại đích của tin nhắn qua giao diện bên ngoài, giúp xác định đích đến của tin nhắn
  uint8_t if_sig; // Quản lý tín hiệu của tin nhắn qua giao diện bên ngoài, giúp xác định loại tin nhắn và cách xử lý nó
} ak_msg_t;
```

### Pool tin nhắn

EDP quản lý 3 loại pool tin nhắn:

| Pool Type | Mục đích | Kích thước mặc định | Dữ liệu |
| ----------- | --------- | ------------------ | -------- |
| **Common** | Tin nhắn thông dụng, kích thước cố định | 8 | 64 bytes |
| **Pure** | Tin nhắn không có dữ liệu hoặc dữ liệu nhỏ | 32 | - |
| **Dynamic** | Tin nhắn với dữ liệu lớn, cấp phát động | 8 | 128 bytes × 4 PDU |

Các kích thước được định cấu hình trong `message.h`:

```c
#define AK_COMMON_MSG_POOL_SIZE         8
#define AK_PURE_MSG_POOL_SIZE           32
#define AK_DYNAMIC_MSG_POOL_SIZE        8
#define AK_COMMON_MSG_DATA_SIZE         64
#define AK_DYNAMIC_DATA_POOL_SIZE       128
#define AK_DYNAMIC_PDU_SIZE             4
```

### Cấp phát và gửi tin nhắn

Phần cấp phát và gửi tin nhắn được implement chung với các logic của task nên không được tách riêng trong phần này, tuy nhiên có thể tham khảo cách sử dụng trong phần quản lý tác vụ ở trên.

### Ví dụ: Giao tiếp giữa hai tác vụ

```c
// Task A gửi dữ liệu cho Task B
void task_a_handler(ak_msg_t* msg) {
  // Kiểm tra đảm bảo msg không phải là NULL
  if (msg == NULL) return;

  // Lưu debug task A đã được gọi
  glb_dbg.task_a_called = 1;

  // Lưu debug msg mà task A đã nhận
  glb_dbg.sig_a_rx = msg->sig;

  // Kiểm tra tín hiệu và gửi msg đến task B
  if (msg->sig == SIG_A_INIT) {
    // Gửi message đến task B
    task_post_common_msg(TASK_B_ID, SIG_B_RUNL, NULL, 0);
  }
}

// Task B nhận xử lý
void task_b_handler(ak_msg_t* msg) {
  // Kiểm tra đảm bảo msg không phải là NULL
  if (msg == NULL) return;

  // Lưu debug task B đã được gọi
  glb_dbg.task_b_called = 1;

  // Lưu debug msg mà task B đã nhận
  glb_dbg.sig_b_rx = msg->sig;

  // Thực hiện vòng lặp incr loop 10 lần
  if (msg->sig == SIG_B_RUNL) {
    for (loop_count = 0; loop_count < 10; loop_count++) {
      // Không làm gì trong vòng lặp, chỉ tăng loop_count để debug
    }
  }
}

// Hàm main để chạy sample
int sample_main() {
  // Lưu debug AKEDP core hoạt động
  glb_dbg.akedp_enable = 1;

  // Khởi động hệ thống task
  if (task_init() != 0) {
    return -1;
  }

  // Khai báo thông số bảng task cho AKEDP core
  task_create((task_t*)task_table);

  // Lưu debug bảng task đã được khởi tạo
  glb_dbg.tsk_tbl_init_done = 1;

  // Post message khởi động đến task A để bắt đầu chuỗi xử lý
  task_post_common_msg(TASK_A_ID, SIG_A_INIT, NULL, 0);

  // Chạy task scheduler
  glb_dbg.test_success = (uint8_t)task_sample_run_once();

  // Kiểm tra kết quả debug
  if (glb_dbg.test_success == 0) {
    glb_dbg.test_success = 0; // Thất bại nếu không có task nào được schedule
  } else {
    glb_dbg.test_success = 1; // Thành công nếu cả hai task đều được schedule
  }

  return 0;
}
```

---

## Quản lý bộ hẹn giờ (Timer Management)

### Khái niệm bộ hẹn giờ

Bộ hẹn giờ cho phép tác vụ yêu cầu hạt nhân gửi một tín hiệu sau một khoảng thời gian nhất định. Hỗ trợ hai chế độ:

- **ONE_SHOT**: Bộ hẹn giờ chỉ kích hoạt một lần
- **PERIODIC**: Bộ hẹn giờ kích hoạt lặp đi lặp lại

### Cấu trúc bộ hẹn giờ

```c
// timer.h
enum timer_type_t {
  TIMER_ONE_SHOT,                       // Kích hoạt một lần
  TIMER_PERIODIC                        // Kích hoạt định kỳ
};

typedef uint8_t timer_sig_t;              // ID tín hiệu bộ hẹn giờ

typedef struct ak_timer_t {
  struct ak_timer_t* next; // Con trỏ đến timer tiếp theo trong pool

  task_id_t des_task_id; // ID của tác vụ đích nhận tin nhắn khi timer kích hoạt
  timer_sig_t sig; // Tín hiệu được gửi đến tác vụ đích khi timer kích hoạt

  uint32_t counter; // Bộ đếm của timer
  uint32_t period; // Chu kỳ của timer
} ak_timer_t;
```

### Tạo và khởi động bộ hẹn giờ

Lưu ý: Phần này sẽ được hệ thống tự động cover với khởi tạo task. Do đó không cần phải gọi `timer_init()` thủ công trong code ứng dụng.

```c
// Hàm khởi tạo Timer mẫu
void timer_init_for_task_a() {
  // Sử dụng timer_set để tạo timer định kỳ
  uint8_t result = timer_set(
    TASK_A_ID,          // ID của Task A
    TASK_A_SIGNAL,      // Tín hiệu gửi đến Task A
    1000,               // Chu kỳ 1000ms
    TIMER_PERIODIC      // Loại timer định kỳ
  );

  if (result != TIMER_RET_OK) {
    FATAL("Timer", 0x01); // Báo lỗi nếu không thể tạo timer
  }
}
```

### Xử lý tín hiệu bộ hẹn giờ

```c
void task_my_app_handler(ak_msg_t* msg) {
  switch (msg->sig) {
    case 0x01:
      printf("User task received signal 0x01\n");
      break;
    default:
      break;
  }
}
```

### Xóa bộ hẹn giờ

```c
#include "timer.h"

// Hủy một timer liên kết với TASK_A_ID và tín hiệu TASK_A_SIGNAL
uint8_t result = timer_remove_attr(TASK_A_ID, TASK_A_SIGNAL);

if (result == TIMER_RET_OK) {
    printf("Timer removed successfully.\n");
} else {
    printf("Failed to remove timer.\n");
}
```

### Ví dụ: Tác vụ có hẹn giờ

```c
void task_sensor_handler(ak_msg_t* msg) {
  static ak_timer_t* read_timer = NULL;

  switch (msg->sig) {
    case SIGNAL_INIT: {
      printf("Sensor task initialized\n");

      // Tạo bộ hẹn giờ để đọc cảm biến mỗi 100ms
      if (!read_timer) {
        read_timer = timer_set(
          AC_SENSOR_TASK_ID,       // Task ID
          TIMER_SIG_READ_SENSOR,   // Signal gửi khi timer hết hạn
          100,                     // 100 tick
          TIMER_PERIODIC           // Chế độ định kỳ
        );
      }

      if (read_timer) {
        printf("Timer created successfully\n");
      } else {
        printf("Failed to create timer\n");
      }
      break;
    }

    case TIMER_SIG_READ_SENSOR: {
      // Đọc giá trị từ cảm biến
      uint32_t sensor_value = read_sensor_value();
      printf("Sensor value: %lu\n", sensor_value);
      break;
    }

    case SIGNAL_STOP: {
      // Dừng và xóa bộ hẹn giờ
      if (read_timer) {
        timer_remove_attr(AC_SENSOR_TASK_ID, TIMER_SIG_READ_SENSOR);
        read_timer = NULL;
        printf("Timer stopped and deleted\n");
      }
      printf("Sensor task stopped\n");
      break;
    }

    default: {
      printf("Unhandled signal: %d\n", msg->sig);
      break;
    }
  }
}
```

---

## Máy trạng thái hữu hạn (FSM)

### Khái niệm FSM

Máy trạng thái hữu tạo (Finite State Machine - FSM) là một mô hình giúp quản lý logic phức tạp bằng cách chia nó thành các trạng thái rõ ràng. Mỗi trạng thái định nghĩa hành động cần thực hiện.

### Cấu trúc FSM trong EDP

```c
// Khai báo con trỏ hàm
typedef void (*state_handler)(ak_msg_t* msg);

// Khai báo cấu trúc FSM
typedef struct {
  state_handler state;
} fsm_t;

// Khai báo hàm để xử lý tin nhắn trong FSM
void fsm_dispatch(fsm_t* me, ak_msg_t* msg);
```

### Tạo FSM

```c
// Định nghĩa các tín hiệu (signals)
#define SIGNAL_INIT       1
#define SIGNAL_READ       2
#define SIGNAL_STOP       3

// Forward declaration các trạng thái
void state_idle(ak_msg_t* msg);
void state_reading(ak_msg_t* msg);

// Định nghĩa FSM
fsm_t sensor_fsm;

// Triển khai các trạng thái
// Trạng thái "Idle"
void state_idle(ak_msg_t* msg) {
  switch (msg->sig) {
    case SIGNAL_INIT:
      printf("Sensor initialized. Transitioning to READING state.\n");
      FSM_TRAN(&sensor_fsm, state_reading); // Chuyển sang trạng thái READING
      break;

    default:
      printf("Unhandled signal in IDLE state: %d\n", msg->sig);
      break;
  }
}

// Trạng thái "Reading"
void state_reading(ak_msg_t* msg) {
  switch (msg->sig) {
    case SIGNAL_READ:
      printf("Reading sensor data...\n");
      // Xử lý đọc cảm biến
      break;

    case SIGNAL_STOP:
      printf("Stopping sensor. Transitioning to IDLE state.\n");
      FSM_TRAN(&sensor_fsm, state_idle); // Chuyển về trạng thái IDLE
      break;

    default:
      printf("Unhandled signal in READING state: %d\n", msg->sig);
      break;
  }
}
```

### Sử dụng FSM

```c
int main() {
  // Khởi tạo FSM với trạng thái ban đầu là IDLE
  FSM(&sensor_fsm, state_idle);

  // Tạo các tin nhắn để gửi đến FSM
  ak_msg_t msg_init = { .sig = SIGNAL_INIT };
  ak_msg_t msg_read = { .sig = SIGNAL_READ };
  ak_msg_t msg_stop = { .sig = SIGNAL_STOP };

  // Gửi các tin nhắn đến FSM
  fsm_dispatch(&sensor_fsm, &msg_init); // Khởi tạo cảm biến
  fsm_dispatch(&sensor_fsm, &msg_read); // Đọc dữ liệu cảm biến
  fsm_dispatch(&sensor_fsm, &msg_stop); // Dừng cảm biến

  return 0;
}
```

---

## Lớp trừu tượng nền tảng (Platform Abstraction)

### Điều khiển ngắt (Interrupt Control)

EDP cung cấp hàm để bảo vệ các phần tới hạn khỏi ngắt:

```c
// platform.h

// Vào phần tới hạn (disable ngắt)
void platform_enter_critical() {
  // Tắt tất cả ngắt
  __disable_irq();
}

// Ra khỏi phần tới hạn (enable ngắt)
void platform_exit_critical() {
  // Bật lại ngắt
  __enable_irq();
}

// Macro tiện ích
#define CRITICAL_SECTION_ENTER()    platform_enter_critical()
#define CRITICAL_SECTION_EXIT()     platform_exit_critical()
```

### Ví dụ: Bảo vệ dữ liệu chia sẻ

```c
// Biến toàn cục được truy cập bởi tác vụ và ngắt
static uint32_t shared_data = 0;

// Cập nhật dữ liệu chia sẻ trong tác vụ
void update_shared_data(uint32_t new_value) {
  CRITICAL_SECTION_ENTER();
  {
      shared_data = new_value;
  }
  CRITICAL_SECTION_EXIT();
}

// Đọc dữ liệu chia sẻ
uint32_t read_shared_data() {
  uint32_t temp;
  
  CRITICAL_SECTION_ENTER();
  {
    temp = shared_data;
  }
  CRITICAL_SECTION_EXIT();
  
  return temp;
}

// Hàm ngắt đơn
void uart_irq_handler() {
  // Không cần bảo vệ vì ngắt không thể lồng nhau
  shared_data++;
}
```

### Quản lý ưu tiên ngắt

```c
// sys_irq.h - Ưu tiên ngắt được định sẵn

enum system_irq_prio_e {
  IRQ_PRIO_UART0_CONSOLE = 2,      // Bộ điều khiển console
  IRQ_PRIO_UART2 = 3,              // UART thứ 2
  IRQ_PRIO_EXTI = 4,               // Ngắt ngoài
  IRQ_PRIO_TIMER_10MS = 5          // Bộ hẹn giờ hệ thống 10ms
};
```

---

## Quản lý ngắt (Interrupt Management)

### Cấu hình ưu tiên ngắt

```c
// Thiết lập ưu tiên cho ngắt UART
NVIC_SetPriority(USART1_IRQn, IRQ_PRIO_UART0_CONSOLE);
NVIC_SetPriority(USART2_IRQn, IRQ_PRIO_UART2);

// Thẻ ưu tiên nhóm (group priority)
// Nhóm 2: 2 bit cho ưu tiên nhóm, 2 bit cho ưu tiên con
NVIC_SetPriorityGrouping(2);
```

### Xử lý ngắt trong EDP

```c
// Ngắt UART0 (console)
void USART1_IRQHandler(void) {
  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
    uint8_t data = USART_ReceiveData(USART1);
    
    // Xử lý dữ liệu nhận được
    // Gửi tin nhắn tới task nếu cần
    ak_msg_t* msg = ak_alloc_msg(AC_UART_IF_TASK_ID, UART_SIG_RX_DATA);
    if (msg && msg->data) {
      *(uint8_t*)msg->data = data;
      ak_post_msg(msg);
    }
  }
}

// Ngắt Timer hệ thống (10ms)
void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    // Cập nhật bộ hẹn giờ
    ak_timer_update();
  }
}
```

---

## Cấu trúc dữ liệu tiện ích (Utility Data Structures)

### FIFO Queue (Hàng đợi FIFO)

```c
#include "fifo.h"

// Tạo hàng đợi FIFO
#define FIFO_SIZE 32
fifo_t my_fifo;
uint8_t fifo_buffer[FIFO_SIZE];

void init_fifo() {
  fifo_init(&my_fifo, fifo_buffer, FIFO_SIZE, sizeof(uint8_t));
}

// Đưa dữ liệu vào hàng đợi
void fifo_put_example() {
  uint8_t data = 0x42;
  if (fifo_put(&my_fifo, &data) == FIFO_OK) {
    printf("Data added to FIFO\n");
  }
}

// Lấy dữ liệu từ hàng đợi
void fifo_get_example() {
  uint8_t data;
  if (fifo_get(&my_fifo, &data) == FIFO_OK) {
    printf("Data from FIFO: 0x%02X\n", data);
  }
}
```

### Ring Buffer (Bộ đệm vòng)

```c
#include "ring_buffer.h"

// Tạo bộ đệm vòng
ring_buffer_t my_ring_buf;
uint8_t rb_data[128];

void init_ring_buffer() {
  ring_buffer_init(&my_ring_buf, rb_data, sizeof(rb_data));
}

// Ghi dữ liệu
void rb_write_example() {
  uint8_t data[] = {0x01, 0x02, 0x03};
  ring_buffer_write(&my_ring_buf, data, sizeof(data));
}

// Đọc dữ liệu
void rb_read_example() {
  uint8_t buffer[16];
  uint32_t bytes_read = ring_buffer_read(&my_ring_buf, buffer, sizeof(buffer));
  printf("Read %lu bytes from ring buffer\n", bytes_read);
}
```

### Log Queue (Hàng đợi ghi nhật ký)

```c
#include "log_queue.h"

log_queue_t system_log;

void init_log_queue() {
  log_queue_init(&system_log);
}

void log_message(const char* format, ...) {
  // Ghi nhật ký tin nhắn
  log_queue_put(&system_log, (uint8_t*)format, strlen(format));
}
```

---

## Ví dụ thực tế (Practical Examples)

### Ví dụ 1: Tác vụ đọc cảm biến định kỳ

```c
// sensor_task.h
#ifndef SENSOR_TASK_H
  #define SENSOR_TASK_H

  // Khai báo thư viện hạt nhân
  #include "ak.h"

  // Định nghĩa ID tác vụ và tín hiệu
  #define AC_SENSOR_TASK_ID               20
  #define AC_SENSOR_INIT                  1
  #define AC_SENSOR_READ                  2
  #define AC_SENSOR_STOP                  3

  // Khai báo hàm xử lý tác vụ
  void task_sensor_handler(ak_msg_t* msg);

#endif // SENSOR_TASK_H

  // sensor_task.c
  #include "sensor_task.h"

  static ak_timer_t* sensor_timer = NULL;

  void task_sensor_handler(ak_msg_t* msg) {
    switch (msg->sig) {
      case AC_SENSOR_INIT: {
        printf("[SENSOR] Initializing sensor task\n");

        // Tạo bộ hẹn giờ để đọc cảm biến mỗi 500ms
        sensor_timer = timer_set(
          AC_SENSOR_TASK_ID,
          AC_SENSOR_READ,
          500,
          TIMER_PERIODIC
        );

        if (sensor_timer) {
          printf("[SENSOR] Timer started\n");
        }
        break;
      }

      case AC_SENSOR_READ: {
        // Đọc giá trị từ cảm biến ADC
        uint32_t adc_value = adc_read_channel(ADC_CHANNEL_TEMP);
        printf("[SENSOR] ADC value: %lu\n", adc_value);

        // Gửi giá trị tới task khác nếu vượt ngưỡng
        if (adc_value > 3000) {
          ak_msg_t* alert_msg = ak_alloc_msg(AC_LIFE_TASK_ID, ALERT_HIGH_TEMP);
          if (alert_msg && alert_msg->data) {
            *(uint32_t*)alert_msg->data = adc_value;
            ak_post_msg(alert_msg);
            printf("[SENSOR] Alert sent\n");
          }
        }
        break;
      }

      case AC_SENSOR_STOP: {
        printf("[SENSOR] Stopping sensor task\n");
        if (sensor_timer) {
          timer_remove_attr(AC_SENSOR_TASK_ID, AC_SENSOR_READ);
          sensor_timer = NULL;
        }
        break;
      }
    }
  }
```

### Ví dụ 2: Tác vụ xử lý UART với FSM

```c
// uart_task.h
#ifndef UART_TASK_H
  #define UART_TASK_H

  // Khai báo thư viện hạt nhân
  #include "ak.h"

  // Định nghĩa ID tác vụ và tín hiệu
  #define AC_UART_TASK_ID                 21
  #define AC_UART_INIT                    1
  #define AC_UART_RX_DATA                 2
  #define AC_UART_SEND_DATA               3

  // Định nghĩa trạng thái cho FSM
  enum uart_state_t {
    UART_STATE_IDLE,
    UART_STATE_RECEIVING,
    UART_STATE_PROCESSING,
    UART_STATE_TRANSMITTING
  };

  void task_uart_handler(ak_msg_t* msg);

#endif // UART_TASK_H

// uart_task.c
#include "uart_task.h"

typedef struct {
  enum uart_state_t state;
  uint8_t buffer[64];
  uint32_t buf_idx;
} uart_context_t;

static uart_context_t uart_ctx = {0};

// Hàm xử lý cho mỗi trạng thái
void uart_state_idle(ak_msg_t* msg) {
  if (msg->sig == AC_UART_RX_DATA) {
    printf("[UART] Idle -> Receiving\n");
    uart_ctx.state = UART_STATE_RECEIVING;
    uart_ctx.buf_idx = 0;
  }
}

void uart_state_receiving(ak_msg_t* msg) {
  if (msg->sig == AC_UART_RX_DATA) {
    if (uart_ctx.buf_idx < sizeof(uart_ctx.buffer)) {
      uart_ctx.buffer[uart_ctx.buf_idx++] = *(uint8_t*)msg->data;

      // Nếu nhận được dòng mới, chuyển sang xử lý
      if (*(uint8_t*)msg->data == '\n') {
        uart_ctx.state = UART_STATE_PROCESSING;
        printf("[UART] Receiving -> Processing\n");
      }
    }
  }
}

void uart_state_processing(ak_msg_t* msg) {
  if (msg->sig == AC_UART_RX_DATA) {
    printf("[UART] Received command: %.*s\n", uart_ctx.buf_idx, uart_ctx.buffer);
    uart_ctx.state = UART_STATE_TRANSMITTING;
    printf("[UART] Processing -> Transmitting\n");
  }
}

void uart_state_transmitting(ak_msg_t* msg) {
  if (msg->sig == AC_UART_RX_DATA) {
    printf("[UART] Response sent\n");
    uart_ctx.state = UART_STATE_IDLE;
    printf("[UART] Transmitting -> Idle\n");
  }
}

void task_uart_handler(ak_msg_t* msg) {
  switch (msg->sig) {
    case AC_UART_INIT: {
      printf("[UART] Task initialized\n");
      uart_ctx.state = UART_STATE_IDLE;
      break;
    }

    case AC_UART_RX_DATA: {
      // Xử lý theo trạng thái hiện tại
      switch (uart_ctx.state) {
        case UART_STATE_IDLE:
          uart_state_idle(msg);
          break;
        case UART_STATE_RECEIVING:
          uart_state_receiving(msg);
          break;
        case UART_STATE_PROCESSING:
          uart_state_processing(msg);
          break;
        case UART_STATE_TRANSMITTING:
          uart_state_transmitting(msg);
          break;
      }
      break;
    }
  }
}
```

### Ví dụ 3: Tiện ích hệ thống

```c
// system_task.c
#include "system.h"
#include "platform.h"

void display_system_info() {
  // Lấy thông tin hệ thống
  system_info_t sys_info = get_system_info();
  
  printf("=== System Information ===\n");
  printf("Flash used: %lu bytes\n", sys_info.flash_used);
  printf("RAM used: %lu bytes\n", sys_info.ram_used);
  printf("Stack available: %lu bytes\n", sys_info.stack_avail);
  printf("Heap available: %lu bytes\n", sys_info.heap_avail);
  printf("CPU clock: %lu Hz\n", sys_info.cpu_clock);
  printf("System tick: %lu ms\n", sys_info.tick);
  printf("Console baudrate: %lu bps\n", sys_info.console_baudrate);
}

void critical_operation() {
  uint32_t critical_data = 0;
  
  // Bảo vệ hoạt động tới hạn
  CRITICAL_SECTION_ENTER();
  {
    // Thực hiện công việc không được ngắt
    critical_data = read_critical_register();
    modify_critical_register(critical_data + 1);
  }
  CRITICAL_SECTION_EXIT();
  
  printf("Critical operation completed safely\n");
}
```

---

## Các lưu ý quan trọng

### 1. An toàn bộ nhớ (Memory Safety)

- Luôn kiểm tra con trỏ NULL trước khi sử dụng
- Giải phóng tin nhắn và bộ hẹn giờ khi không còn cần
- Tránh truy cập ngoài giới hạn bộ đệm

### 2. Ưu tiên tác vụ (Task Priority)

- Mức ưu tiên 0 = cao nhất, mức 7 = thấp nhất
- Tác vụ quan trọng (UART, Timer) nên có ưu tiên cao hơn
- Tránh tác vụ ưu tiên cao chạy quá lâu

### 3. Quản lý tài nguyên (Resource Management)

- Số lượng tin nhắn hạn chế - thiết kế cẩn thận
- Bộ đệm dữ liệu cấp phát tĩnh - tránh phân mảnh bộ nhớ
- Kiểm tra việc cấp phát thành công

---

## Tài liệu tham khảo

1. **ak.h** - Định nghĩa khung AK chính
2. **task.h** - Quản lý tác vụ
3. **message.h** - Hệ thống tin nhắn
4. **timer.h** - Quản lý bộ hẹn giờ
5. **fsm.h** - Máy trạng thái hữu hạn
6. **platform.h** - Trừu tượng hóa nền tảng
7. **system.h** - Thông tin hệ thống
