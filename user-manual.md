# Hướng dẫn sử dụng EDP Core

**Phiên bản**: 1.0  
**Tác giả**: Shang Huang  
**Ngày cập nhật**: 09/04/2026

---

## Ghi chú

Hiện tại theo kiểm tra tài liệu và đối chiếu với code thì có 1 số sai lệch cần được chỉnh sửa.

Tuy nhiên các nguồn tham khảo vẫn có phần lớn giá trị về mặt tham khảo kiến trúc và cách sử dụng EDP Core, nên sẽ giữ nguyên nội dung hiện tại của tài liệu.

Các phần sai lệch sẽ được chỉnh sửa lại ở các commit sau để đảm bảo tính chính xác của tài liệu, đồng thời sẽ bổ sung thêm các ví dụ thực tế để minh họa cách sử dụng EDP Core trong các tình huống cụ thể.

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

### Định nghĩa EDP Core

**EDP (Embedded Development Platform)** là một nền tảng phát triển nhúng được thiết kế cho các hệ thống nhúng thời gian thực. EDP cung cấp một kiến trúc phần mềm hoàn chỉnh bao gồm:

- **Hạt nhân**: Bộ lập lịch tác vụ (Task Scheduler), quản lý tin nhắn, và bộ hẹn giờ
- **Tầng ứng dụng (Application Layer)**: Các tác vụ ứng dụng (LIFE, UART IF, Debug tasks)
- **Tầng hệ thống (System Layer)**: Trừu tượng hóa nền tảng, quản lý ngắt, I/O điều khiển
- **Thư viện công cụ (Utility Libraries)**: Cấu trúc dữ liệu (FIFO, Ring Buffer), tiện ích in ra

Ngoài ra EDP còn đại diện cho Event-Driven Programming, một mô hình lập trình trong đó luồng điều khiển được xác định bởi các sự kiện (events) như tin nhắn, ngắt, hoặc bộ hẹn giờ.

### Đặc điểm chính

| Đặc điểm | Mô tả |
| ---------- | ------- |
| **Tác vụ đa ưu tiên** | 8 mức ưu tiên (0-7), hỗ trợ tác vụ idle |
| **Giao tiếp tin nhắn** | Hệ thống tin nhắn bất đồng bộ với 3 loại pool (common, pure, dynamic) |
| **Quản lý thời gian** | Bộ hẹn giờ với hỗ trợ one-shot và periodic |
| **Trừu tượng hóa phần cứng** | Lớp platform abstraction có thể tái sử dụng trên các MCU khác |
| **Hỗ trợ FSM** | Máy trạng thái hữu hạn để quản lý logic phức tạp |
| **An toàn luồng** | Cực tiểu hóa các phần tới hạn (critical sections) để tăng hiệu suất |

---

## Kiến trúc EDP Core

### Sơ đồ kiến trúc

```mermaid
┌─────────────────────────────────────────────────────────┐
│            Application Layer (app/)                     │
│  (LIFE Task, UART IF Task, Debug Task, Interface, ...)  │
├─────────────────────────────────────────────────────────┤
│            AKEDP Core (ak/)                             │
│  ┌──────────────┬──────────────┬──────────────────────┐ │
│  │   Task Mgmt  │ Message Mgmt │   Timer Mgmt | FSM   │ │
│  └──────────────┴──────────────┴──────────────────────┘ │
├─────────────────────────────────────────────────────────┤
│         System & Platform Layer (sys/)                  │
│  ┌──────────────┬──────────────┬──────────────────────┐ │
│  │ Interrupt    │  Platform    │   I/O & Device       │ │
│  │ Management   │  Abstraction │   Drivers (UART,LED) │ │
│  └──────────────┴──────────────┴──────────────────────┘ │
├─────────────────────────────────────────────────────────┤
│        Utility Libraries (common/)                      │
│  ┌──────────────┬──────────────┬──────────────────────┐ │
│  │   Containers │   Utils      │    xprintf/Logging   │ │
│  │  (FIFO, RB)  │              │                      │ │
│  └──────────────┴──────────────┴──────────────────────┘ │
├─────────────────────────────────────────────────────────┤
│         STM32 HAL & Hardware Layer                      │
│  (stm32f1xx_hal.h, CMSIS, Device-specific)              │
└─────────────────────────────────────────────────────────┘
```

### Cấu trúc thư mục

```file_structure
EDP/
├── ak/                          # Hạt nhân Active Kernel
│   ├── inc/                     # Header files chính
│   │   ├── ak.h                 # Định nghĩa cơ bản khung Active Kernel
│   │   ├── task.h               # Quản lý tác vụ
│   │   ├── message.h            # Hệ thống tin nhắn
│   │   ├── timer.h              # Quản lý bộ hẹn giờ
│   │   ├── fsm.h                # Máy trạng thái hữu hạn
│   │   ├── port.h               # Cấu hình và cấp phát động
│   │   └── tsm.h                # Task State Machine
│   ├── src/                     # Triển khai
│   │   ├── task.c
│   │   ├── message.c
│   │   ├── timer.c
│   │   └── fsm.c
│   ├── doc/                     # Tài liệu (logo, references)
│   └── ak.cfg.mk                # Cấu hình xây dựng
│
├── app/                         # Tầng ứng dụng
│   ├── app.h/cpp                # Định nghĩa toàn bộ hoạt động với EDP
│   ├── task_if.h/cpp            # Tác vụ giao thức để truyền message từ các interface vào hệ thống
│   └── ... (các tác vụ khác)
│
├── sys/                         # Tầng hệ thống
│   ├── platform.h               # Điều khiển ngắt, trừu tượng nền tảng
│   ├── system.h                 # Thông tin hệ thống
│   ├── sys_cfg.h                # Cấu hình hệ thống
│   ├── sys_boot.h/c             # Mã khởi động
│   ├── sys_dbg.h/c              # Tiện ích debug
│   ├── io_cfg.h                 # Cấu hình I/O
│   └── ... (các tiếp liệu khác)
│
└── common/                      # Thư viện công cụ
    ├── container/
    │   ├── fifo.h/c             # Hàng đợi FIFO
    │   ├── ring_buffer.h/c       # Bộ đệm vòng
    │   └── log_queue.h/c         # Hàng đợi ghi nhật ký
    └── utils.h/c                # Các hàm tiện ích
```

---

## Quản lý tác vụ (Task Management)

### Khái niệm cơ bản

Một **tác vụ (task)** là một đơn vị công việc độc lập có thể được lập lịch bởi bộ lập lịch của hạt nhân AK. EDP hỗ trợ hai loại tác vụ:

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
struct task_t {
    task_id_t id;                             // ID tác vụ
    task_pri_t pri;                           // Ưu tiên (0=cao nhất, 7=thấp nhất)
    pf_task task;                             // Hàm xử lý
};

struct task_polling_t {
    task_id_t id;
    uint8_t ability;                          // Khả năng: 1=enabled, 0=disabled
    pf_task_polling task_polling;
};
```

### ID tác vụ tiêu chuẩn

```c
// ak.h - Các ID tác vụ được định sẵn
#define AK_TASK_IDLE_ID             0      // Tác vụ idle (chạy khi không có gì)
#define AK_TASK_TIMER_ID            1      // Tác vụ bộ hẹn giờ (nội tại)

// Tác vụ ứng dụng
#define AC_LIFE_TASK_ID             2      // Tác vụ giám sát sức khỏe hệ thống
#define AC_UART_IF_TASK_ID          3      // Tác vụ giao diện UART
#define AC_IF_TASK_ID               4      // Tác vụ giao diện chung
#define AC_DBG_TASK_ID              5      // Tác vụ debug
```

### Tạo một tác vụ mới

#### Ví dụ 1: Tác vụ xử lý tin nhắn (Message-driven)

```c
// header file: task_my_app.h
#ifndef TASK_MY_APP_H
  #define TASK_MY_APP_H

  // Khai báo thư viện hạt nhân
  #include "ak.h"

  // Định nghĩa ID tác vụ
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
  switch (msg->sig) {
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

Trong hàm khởi tạo hệ thống (thường ở `main.c` hoặc `sys_boot.c`):

```c
#include "task_my_app.h"
#include "task_polling_example.h"

// Đăng ký tác vụ xử lý tin nhắn
ak_task_register(AC_MY_APP_TASK_ID, 3, task_my_app_handler);

// Đăng ký tác vụ polling
ak_task_polling_register(AC_POLLING_TASK_ID, 1, task_polling_example);

// Gửi tín hiệu khởi tạo
ak_msg_t* init_msg = ak_alloc_msg(AC_MY_APP_TASK_ID, AC_MY_APP_INIT);
ak_post_msg(init_msg);
```

---

## Hệ thống tin nhắn (Message System)

### Khái niệm tin nhắn

Hệ thống tin nhắn là cơ chế truyền thông chính giữa các tác vụ trong EDP. Các tin nhắn được gửi không đồng bộ, cho phép tác vụ sẽ xử lý chúng theo thứ tự ưu tiên.

### Cấu trúc tin nhắn

```c
// message.h
typedef struct {
  uint8_t task_id;                      // Task nhận tin nhắn
  uint8_t sig;                          // Tín hiệu (signal/event type)
  uint8_t state;                        // Trạng thái: AK_MSG_OK, AK_MSG_NG
  uint8_t res_required;                 // Cần phản hồi: 1=có, 0=không
  uint8_t* data;                        // Con trỏ dữ liệu
  uint32_t data_size;                   // Kích thước dữ liệu
  uint8_t sender_task_id;               // Task gửi
  uint32_t wait_time;                   // Thời gian chờ (tính bằng tick)
  uint32_t exe_time;                    // Thời gian thực thi
  struct ak_msg_t* next;                // Liên kết danh sách
  // ... các trường khác
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

#### Cấp phát tin nhắn

```c
// Cấp phát tin nhắn bình thường (common pool)
ak_msg_t* msg = ak_alloc_msg(destination_task_id, signal);

// Kiểm tra việc cấp phát thành công
if (msg == NULL) {
  // Xử lý lỗi: không có bộ nhớ
  printf("Failed to allocate message\n");
  return;
}

// Thiết lập dữ liệu nếu cần
if (msg->data) {
  uint8_t* data = (uint8_t*)msg->data;
  data[0] = 0x42;
  data[1] = 0x43;
}
```

#### Gửi tin nhắn

```c
// Gửi tin nhắn ngay lập tức
ak_post_msg(msg);

// Gửi tin nhắn sau độ trễ (tính bằng millisecond)
ak_post_msg_delay(msg, 100);  // Gửi sau 100ms

// Yêu cầu phản hồi (response required)
msg->res_required = 1;
ak_post_msg(msg);
```

### Ví dụ: Giao tiếp giữa hai tác vụ

```c
// Task A gửi dữ liệu cho Task B
void task_a_handler(ak_msg_t* msg) {
  switch (msg->sig) {
    case SIGNAL_SEND_TO_B: {
      // Tạo tin nhắn cho Task B
      ak_msg_t* new_msg = ak_alloc_msg(TASK_B_ID, SIGNAL_PROCESS_DATA);
      if (new_msg) {
        // Sao chép dữ liệu
        uint8_t* src_data = (uint8_t*)msg->data;
        uint8_t* dst_data = (uint8_t*)new_msg->data;
        dst_data[0] = src_data[0] + 1;
        dst_data[1] = src_data[1] + 2;
        
        // Gửi
        ak_post_msg(new_msg);
        printf("Message sent to Task B\n");
      }
      break;
    }
  }
}

// Task B nhận xử lý
void task_b_handler(ak_msg_t* msg) {
  switch (msg->sig) {
    case SIGNAL_PROCESS_DATA: {
      uint8_t* data = (uint8_t*)msg->data;
      printf("Task B received: [%d, %d]\n", data[0], data[1]);
      break;
    }
  }
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

struct ak_timer_t {
  struct ak_timer_t* next;              // Liên kết danh sách
  task_id_t des_task_id;                // Task đích
  timer_sig_t sig;                      // Tín hiệu (signal ID)
  uint32_t counter;                     // Bộ đếm thời gian
  uint32_t period;                      // Chu kỳ (tính bằng tick)
  enum timer_type_t type;               // Loại bộ hẹn giờ
};
```

### Tạo và khởi động bộ hẹn giờ

```c
// Tạo bộ hẹn giờ one-shot (kích hoạt một lần)
ak_timer_t* timer = ak_timer_create(
  AC_MY_APP_TASK_ID,                    // Task nhận tín hiệu
  TIMER_SIG_PROCESS,                    // Tín hiệu
  1000,                                 // Độ trễ: 1000 tick (~1 giây nếu tick là 1ms)
  TIMER_ONE_SHOT                        // Chế độ
);

// Khởi động bộ hẹn giờ
if (timer) {
  ak_timer_start(timer);
}

// Tạo bộ hẹn giờ định kỳ (kích hoạt lặp lại)
ak_timer_t* periodic_timer = ak_timer_create(
  AC_LIFE_TASK_ID,
  TIMER_SIG_HEALTH_CHECK,
  5000,                                 // Chu kỳ: 5000 tick (~5 giây)
  TIMER_PERIODIC
);

if (periodic_timer) {
  ak_timer_start(periodic_timer);
}
```

### Xử lý tín hiệu bộ hẹn giờ

```c
void task_my_app_handler(ak_msg_t* msg) {
  switch (msg->sig) {
    case TIMER_SIG_PROCESS: {
      // Bộ hẹn giờ đã hết thời gian
      printf("Timer expired!\n");
      // Thực hiện công việc đã lên lịch
      break;
    }
  }
}
```

### Dừng và xóa bộ hẹn giờ

```c
// Dừng bộ hẹn giờ
if (timer) {
  ak_timer_stop(timer);
}

// Xóa bộ hẹn giờ
if (timer) {
  ak_timer_delete(timer);
  timer = NULL;
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
      read_timer = ak_timer_create(
        AC_SENSOR_TASK_ID,
        TIMER_SIG_READ_SENSOR,
        100,                      // 100 tick
        TIMER_PERIODIC
      );
      
      if (read_timer) {
        ak_timer_start(read_timer);
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
      // Dừng bộ hẹn giờ
      if (read_timer) {
        ak_timer_stop(read_timer);
        ak_timer_delete(read_timer);
        read_timer = NULL;
      }
      printf("Sensor task stopped\n");
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
// fsm.h
typedef void (*state_handler)(ak_msg_t* msg);    // Hàm xử lý trạng thái

typedef struct {
  state_handler state;                         // Trạng thái hiện tại
} fsm_t;
```

### Tạo FSM

```c
// Định nghĩa các trạng thái
enum device_state_t {
  STATE_IDLE,
  STATE_INITIALIZING,
  STATE_RUNNING,
  STATE_ERROR,
  STATE_SHUTDOWN
};

// Hàm xử lý cho mỗi trạng thái
void state_idle_handler(ak_msg_t* msg) {
  printf("Device is IDLE\n");
  // Xử lý logic ở trạng thái IDLE
}

void state_initializing_handler(ak_msg_t* msg) {
  printf("Device is INITIALIZING\n");
  // Xử lý logic ở trạng thái INITIALIZING
}

void state_running_handler(ak_msg_t* msg) {
  printf("Device is RUNNING\n");
  // Xử lý logic ở trạng thái RUNNING
}

void state_error_handler(ak_msg_t* msg) {
  printf("Device is in ERROR state\n");
  // Xử lý logic ở trạng thái ERROR
}

void state_shutdown_handler(ak_msg_t* msg) {
  printf("Device is SHUTDOWN\n");
  // Xử lý logic ở trạng thái SHUTDOWN
}
```

### Sử dụng FSM

```c
typedef struct {
  fsm_t fsm;
  enum device_state_t current_state;
} device_context_t;

device_context_t device;

void initialize_device_fsm() {
  device.current_state = STATE_IDLE;
  device.fsm.state = state_idle_handler;
}

void transition_state(enum device_state_t new_state) {
  device.current_state = new_state;
  
  switch (new_state) {
    case STATE_IDLE:
      device.fsm.state = state_idle_handler;
      break;
    case STATE_INITIALIZING:
      device.fsm.state = state_initializing_handler;
      break;
    case STATE_RUNNING:
      device.fsm.state = state_running_handler;
      break;
    case STATE_ERROR:
      device.fsm.state = state_error_handler;
      break;
    case STATE_SHUTDOWN:
      device.fsm.state = state_shutdown_handler;
      break;
  }
}

void device_task_handler(ak_msg_t* msg) {
  // Gọi hàm xử lý cho trạng thái hiện tại
  if (device.fsm.state) {
    device.fsm.state(msg);
  }
  
  // Xử lý chuyển đổi trạng thái dựa trên tín hiệu
  switch (msg->sig) {
    case SIGNAL_START:
      transition_state(STATE_INITIALIZING);
      break;
    case SIGNAL_INIT_COMPLETE:
      transition_state(STATE_RUNNING);
      break;
    case SIGNAL_ERROR:
      transition_state(STATE_ERROR);
      break;
    case SIGNAL_STOP:
      transition_state(STATE_SHUTDOWN);
      break;
  }
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
        sensor_timer = ak_timer_create(
          AC_SENSOR_TASK_ID,
          AC_SENSOR_READ,
          500,
          TIMER_PERIODIC
        );
        
        if (sensor_timer) {
          ak_timer_start(sensor_timer);
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
          ak_timer_stop(sensor_timer);
          ak_timer_delete(sensor_timer);
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

### 4. Gỡ lỗi và ghi nhật ký (Debugging)

- Sử dụng `printf()` cho thông tin gỡ lỗi
- Ghi lại thời gian chờ và thực thi tin nhắn
- Sử dụng định nghĩa `AK_TASK_DEBUG` để kích hoạt chế độ gỡ lỗi

---

## Tài liệu tham khảo

1. **ak.h** - Định nghĩa khung AK chính
2. **task.h** - Quản lý tác vụ
3. **message.h** - Hệ thống tin nhắn
4. **timer.h** - Quản lý bộ hẹn giờ
5. **fsm.h** - Máy trạng thái hữu hạn
6. **platform.h** - Trừu tượng hóa nền tảng
7. **system.h** - Thông tin hệ thống
