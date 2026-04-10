/*
 * sample.h
 *
 *  Created on: Apr 10, 2026
 *      Author: shanghuang
 */

#ifndef SAMPLE_H_
	#define SAMPLE_H_

	// Khai báo các thư viện sử dụng
	#include <stdio.h>
	#include <string.h>
	#include "stdint.h"
	#include "ak.h"
	#include "message.h"
	#include "task.h"
	#include "task_list.h"

	// Định nghĩa ID cho task A
	#define TASK_A_ID (0u)
	#define TASK_B_ID (1u)

	// Định nghĩa các tín hiệu sử dụng trong message
	#define SIG_A_INIT (0xA0u)
	#define SIG_B_RUNL (0xB0u)

	// Khai báo bộ debug tín hiệu
	typedef struct dbg_t {
		// Quản lý debug chung
		uint8_t akedp_enable;
		uint8_t tsk_tbl_init_done;
		uint8_t test_success;

		// Quản lý debug cho task A
		uint8_t task_a_called;
		uint8_t sig_a_rx;

		// Quản lý debug cho task B
		uint8_t task_b_called;
		uint8_t sig_b_rx;
	} dbg_t;

	// Khai báo biến debug toàn cục
	extern volatile dbg_t glb_dbg;

	// Khai báo bảng lưu task
	extern task_t task_table[];

	// Khai báo prototype cho các hàm xử lý task
	void task_a_handler(ak_msg_t* msg);
	void task_b_handler(ak_msg_t* msg);

	// Khai báo biến để task B thực hiện vòng incr loop 10 lần
	extern volatile uint8_t loop_count;

	// Khai báo hàm main để chạy sample
	int sample_main();

#endif /* SAMPLE_H_ */
