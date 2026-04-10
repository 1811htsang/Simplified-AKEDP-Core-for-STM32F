/*
 * sample.c
 *
 *  Created on: Apr 10, 2026
 *      Author: shanghuang
 */

// Khai báo thư viện sử dụng
#include "sample.h"

// Khai báo biến debug toàn cục
volatile dbg_t glb_dbg = {0};

// Khai báo biến đếm
volatile uint8_t loop_count = 1;

// Khai báo bảng lưu task
task_t task_table[3] = {
  {TASK_A_ID, TASK_PRI_LEVEL_2, task_a_handler},
  {TASK_B_ID, TASK_PRI_LEVEL_2, task_b_handler},
  {AK_TASK_EOT_ID, TASK_PRI_LEVEL_0, (pf_task)0} // Báo End-Of-Table
};

// Hàm xử lý task A
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

// Hàm xử lý task B
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
