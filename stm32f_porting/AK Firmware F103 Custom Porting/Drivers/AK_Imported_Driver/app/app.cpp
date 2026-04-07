/**
 ******************************************************************************
 * @author: GaoKong
 * @date:   13/08/2016
 ******************************************************************************
**/
/* kernel include */
#include "ak.h"
#include "message.h"
#include "timer.h"
#include "fsm.h"

/* driver include */
#include "led.h"

/* app include */
#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_shell.h"
#include "task_life.h"
#include "task_if.h"
#include "task_uart_if.h"
#include "task_display.h"

/* sys include */
#include "sys_boot.h"
#include "sys_irq.h"
#include "sys_io.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"

/* arduino include */
#include "SPI.h"
#include "WString.h"
#include "HardwareSerial.h"
#include "ArduinoJson.h"

/* common include */
#include "screen_manager.h"

/* ----------------------- Platform includes --------------------------------*/

/* ----------------------- Json includes ------------------------------------*/
//#include "json.hpp"

#if defined(RELEASE)
const char* app_run_mode = "RELEASE";
#else
static const char* app_run_mode = "DEBUG";
#endif

const app_info_t app_info { \
	APP_MAGIC_NUMBER, \
			APP_VER, \
};

static void app_start_timer();
static void app_init_state_machine();
static void app_task_init();
static uint8_t app_run_edp_init_probe();
static int app_run_edp_two_task_debug();
static void app_task_edp_dbg_a(ak_msg_t* msg);
static void app_task_edp_dbg_b(ak_msg_t* msg);

/* Tín hiệu riêng dùng cho startup probe để kiểm tra luồng post/remove queue. */
#define APP_EDP_PROBE_SIG (0xF1u)
#define APP_EDP_DBG_SIG_START (0xA1u)
#define APP_EDP_DBG_SIG_FORWARD (0xB1u)

typedef enum {
	APP_EDP_DBG_TASK_A_ID = 0,
	APP_EDP_DBG_TASK_B_ID,
	APP_EDP_DBG_TASK_EOT_ID
} app_edp_dbg_task_id_t;

/*
 * Trạng thái debug cho luồng chuyển message giữa 2 task nội bộ.
 * Tập trung quan sát các biến này để kiểm tra vòng đời message và thứ tự xử lý task.
 */
typedef struct {
	uint8_t init_ret;
	uint8_t created_ret;
	uint8_t first_dispatch_ret;
	uint8_t second_dispatch_ret;
	uint8_t deinit_done;

	uint8_t task_a_rx_count;
	uint8_t task_b_rx_count;
	uint8_t transfer_done;

	uint8_t first_sig_seen;
	uint8_t second_sig_seen;
	uint8_t sample_len;
	char sample_payload[32];
} app_edp_two_task_dbg_t;

volatile app_edp_two_task_dbg_t g_app_edp_two_task_dbg = {
	AK_DISABLE,
	AK_DISABLE,
	AK_DISABLE,
	AK_DISABLE,
	AK_DISABLE,
	0,
	0,
	AK_DISABLE,
	0,
	0,
	0,
	{0}
};

static task_t app_edp_dbg_task_table[] = {
	{APP_EDP_DBG_TASK_A_ID, TASK_PRI_LEVEL_2, app_task_edp_dbg_a},
	{APP_EDP_DBG_TASK_B_ID, TASK_PRI_LEVEL_2, app_task_edp_dbg_b},
	{AK_TASK_EOT_ID, TASK_PRI_LEVEL_0, (pf_task)0}
};

static task_polling_t app_edp_dbg_polling_table[] = {
	{AK_TASK_POLLING_EOT_ID, AK_DISABLE, (pf_task_polling)0}
};

/*
 * Snapshot chẩn đoán giai đoạn bring-up của AK-EDP.
 * Các trường này được thiết kế để theo dõi trực tiếp trong cửa sổ watch của debugger.
 */
typedef struct {
	uint8_t app_info_valid;
	uint8_t task_table_valid;
	uint8_t polling_table_valid;
	uint8_t task_count;
	uint8_t polling_task_count;
	task_id_t first_invalid_task_id;
	task_id_t first_invalid_polling_id;
	task_pri_t first_invalid_task_pri;
	uint8_t first_invalid_polling_ability;
	int edp_init_ret;
	uint8_t edp_probe_valid;
	uint8_t created_task_table_size;
	uint8_t expected_task_table_size;
	uint32_t pure_pool_used_before_queue_probe;
	uint32_t pure_pool_used_after_queue_probe;
	uint8_t queue_probe_removed_count;
	uint8_t queue_probe_ok;
	const task_t* task_tbl_ptr;
	const task_polling_t* polling_tbl_ptr;
} app_startup_diag_t;

volatile app_startup_diag_t g_app_startup_diag = {
	AK_DISABLE,
	AK_DISABLE,
	AK_DISABLE,
	0,
	0,
	AK_TASK_EOT_ID,
	AK_TASK_POLLING_EOT_ID,
	TASK_PRI_LEVEL_0,
	AK_DISABLE,
	-1,
	AK_DISABLE,
	0,
	0,
	0,
	0,
	0,
	AK_DISABLE,
	(task_t*)0,
	(task_polling_t*)0
};

/*
 * Chạy probe khởi tạo AK-EDP theo kiểu không chặn:
 * 1) khởi tạo lõi task và đăng ký bảng task/polling,
 * 2) post một message kiểm thử,
 * 3) remove lại message đó khỏi queue,
 * 4) xác nhận mức dùng pool quay về giá trị ban đầu.
 */
static uint8_t app_run_edp_init_probe() {
	g_app_startup_diag.edp_probe_valid = AK_DISABLE;
	g_app_startup_diag.created_task_table_size = 0;
	g_app_startup_diag.expected_task_table_size = g_app_startup_diag.task_count;
	g_app_startup_diag.queue_probe_removed_count = 0;
	g_app_startup_diag.queue_probe_ok = AK_DISABLE;

	g_app_startup_diag.pure_pool_used_before_queue_probe = 0;
	g_app_startup_diag.pure_pool_used_after_queue_probe = 0;

	g_app_startup_diag.edp_init_ret = task_init();
	if (g_app_startup_diag.edp_init_ret != 0) {
		return AK_DISABLE;
	}

	task_create((task_t*)app_task_table);
	task_polling_create((task_polling_t*)app_task_polling_table);

	g_app_startup_diag.created_task_table_size = get_task_table_size();
	if (g_app_startup_diag.created_task_table_size != g_app_startup_diag.expected_task_table_size) {
		return AK_DISABLE;
	}

	g_app_startup_diag.pure_pool_used_before_queue_probe = get_pure_msg_pool_used();
	task_post_pure_msg(AC_TASK_SYSTEM_ID, APP_EDP_PROBE_SIG);
	g_app_startup_diag.queue_probe_removed_count = task_remove_msg(AC_TASK_SYSTEM_ID, APP_EDP_PROBE_SIG);
	g_app_startup_diag.pure_pool_used_after_queue_probe = get_pure_msg_pool_used();

	g_app_startup_diag.queue_probe_ok =
			(g_app_startup_diag.queue_probe_removed_count == 1)
			&& (g_app_startup_diag.pure_pool_used_before_queue_probe == g_app_startup_diag.pure_pool_used_after_queue_probe)
			? AK_ENABLE
			: AK_DISABLE;

	g_app_startup_diag.edp_probe_valid = (g_app_startup_diag.queue_probe_ok == AK_ENABLE) ? AK_ENABLE : AK_DISABLE;
	return g_app_startup_diag.edp_probe_valid;
}

/*
 * Task A: nhận message khởi động và chuyển tiếp một message mẫu sang Task B.
 */
static void app_task_edp_dbg_a(ak_msg_t* msg) {
	if (msg == (ak_msg_t*)0) {
		return;
	}

	g_app_edp_two_task_dbg.task_a_rx_count++;
	g_app_edp_two_task_dbg.first_sig_seen = msg->sig;

	if (msg->sig == APP_EDP_DBG_SIG_START) {
		uint8_t sample_data[] = "EDP_MSG_A_TO_B";
		task_post_common_msg(APP_EDP_DBG_TASK_B_ID, APP_EDP_DBG_SIG_FORWARD, sample_data, (uint8_t)(sizeof(sample_data) - 1));
	}
}

/*
 * Task B: nhận message chuyển tiếp từ Task A và lưu payload để debug.
 */
static void app_task_edp_dbg_b(ak_msg_t* msg) {
	if (msg == (ak_msg_t*)0) {
		return;
	}

	g_app_edp_two_task_dbg.task_b_rx_count++;
	g_app_edp_two_task_dbg.second_sig_seen = msg->sig;

	if (msg->sig == APP_EDP_DBG_SIG_FORWARD) {
		uint8_t len = get_data_len_common_msg(msg);
		uint8_t* data = get_data_common_msg(msg);

		if (len >= sizeof(g_app_edp_two_task_dbg.sample_payload)) {
			len = (uint8_t)(sizeof(g_app_edp_two_task_dbg.sample_payload) - 1);
		}

		memcpy((void*)g_app_edp_two_task_dbg.sample_payload, (const void*)data, len);
		g_app_edp_two_task_dbg.sample_payload[len] = 0;
		g_app_edp_two_task_dbg.sample_len = len;
		g_app_edp_two_task_dbg.transfer_done = AK_ENABLE;
	}
}

/*
 * Kịch bản debug trọn vẹn không dùng ngoại vi:
 * - Khởi tạo component AK-EDP,
 * - Tạo 2 task debug,
 * - Chuyển 1 message mẫu từ task A sang task B,
 * - Deinit hệ thống về trạng thái trước khi khởi tạo.
 */
static int app_run_edp_two_task_debug() {
	memset((void*)&g_app_edp_two_task_dbg, 0, sizeof(app_edp_two_task_dbg_t));

	if (task_init() != 0) {
		return -10;
	}
	g_app_edp_two_task_dbg.init_ret = AK_ENABLE;

	task_create((task_t*)app_edp_dbg_task_table);
	task_polling_create((task_polling_t*)app_edp_dbg_polling_table);
	g_app_edp_two_task_dbg.created_ret = AK_ENABLE;

	/* Đẩy message đầu tiên vào task A để kích hoạt chuỗi chuyển tiếp A -> B. */
	task_post_pure_msg(APP_EDP_DBG_TASK_A_ID, APP_EDP_DBG_SIG_START);

	/* Lần dispatch 1: xử lý task A, task A sẽ forward message sang task B. */
	g_app_edp_two_task_dbg.first_dispatch_ret = (uint8_t)task_debug_run_once();

	/* Lần dispatch 2: xử lý task B để hoàn tất nhận message mẫu. */
	g_app_edp_two_task_dbg.second_dispatch_ret = (uint8_t)task_debug_run_once();

	if ((g_app_edp_two_task_dbg.task_a_rx_count == 0)
			|| (g_app_edp_two_task_dbg.task_b_rx_count == 0)
			|| (g_app_edp_two_task_dbg.transfer_done == AK_DISABLE)) {
		task_deinit();
		g_app_edp_two_task_dbg.deinit_done = AK_ENABLE;
		return -11;
	}

	task_deinit();
	g_app_edp_two_task_dbg.deinit_done = AK_ENABLE;

	return 0;
}

static uint8_t app_validate_task_table(const task_t* task_tbl) {
	task_id_t prev_id = 0;
	uint8_t is_first_entry = AK_ENABLE;

	g_app_startup_diag.task_count = 0;
	g_app_startup_diag.first_invalid_task_id = AK_TASK_EOT_ID;
	g_app_startup_diag.first_invalid_task_pri = TASK_PRI_LEVEL_0;

	if (task_tbl == (task_t*)0) {
		g_app_startup_diag.first_invalid_task_id = AK_TASK_EOT_ID;
		return AK_DISABLE;
	}

	for (uint8_t i = 0; i < 64; i++) {
		if (task_tbl[i].id == AK_TASK_EOT_ID) {
			if (task_tbl[i].task != (pf_task)0) {
				g_app_startup_diag.first_invalid_task_id = task_tbl[i].id;
				return AK_DISABLE;
			}

			return AK_ENABLE;
		}

		if (task_tbl[i].task == (pf_task)0) {
			g_app_startup_diag.first_invalid_task_id = task_tbl[i].id;
			return AK_DISABLE;
		}

		if (task_tbl[i].pri >= TASK_PRI_MAX_SIZE) {
			g_app_startup_diag.first_invalid_task_id = task_tbl[i].id;
			g_app_startup_diag.first_invalid_task_pri = task_tbl[i].pri;
			return AK_DISABLE;
		}

		if ((is_first_entry == AK_DISABLE) && (task_tbl[i].id <= prev_id)) {
			g_app_startup_diag.first_invalid_task_id = task_tbl[i].id;
			return AK_DISABLE;
		}

		prev_id = task_tbl[i].id;
		is_first_entry = AK_DISABLE;
		g_app_startup_diag.task_count++;
	}

	g_app_startup_diag.first_invalid_task_id = AK_TASK_EOT_ID;
	return AK_DISABLE;
}

static uint8_t app_validate_polling_table(const task_polling_t* polling_tbl) {
	task_id_t prev_id = 0;
	uint8_t is_first_entry = AK_ENABLE;

	g_app_startup_diag.polling_task_count = 0;
	g_app_startup_diag.first_invalid_polling_id = AK_TASK_POLLING_EOT_ID;
	g_app_startup_diag.first_invalid_polling_ability = AK_DISABLE;

	if (polling_tbl == (task_polling_t*)0) {
		g_app_startup_diag.first_invalid_polling_id = AK_TASK_POLLING_EOT_ID;
		return AK_DISABLE;
	}

	for (uint8_t i = 0; i < 32; i++) {
		if (polling_tbl[i].id == AK_TASK_POLLING_EOT_ID) {
			if ((polling_tbl[i].ability != AK_DISABLE) || (polling_tbl[i].task_polling != (pf_task_polling)0)) {
				g_app_startup_diag.first_invalid_polling_id = polling_tbl[i].id;
				g_app_startup_diag.first_invalid_polling_ability = polling_tbl[i].ability;
				return AK_DISABLE;
			}

			return AK_ENABLE;
		}

		if ((polling_tbl[i].ability != AK_ENABLE) && (polling_tbl[i].ability != AK_DISABLE)) {
			g_app_startup_diag.first_invalid_polling_id = polling_tbl[i].id;
			g_app_startup_diag.first_invalid_polling_ability = polling_tbl[i].ability;
			return AK_DISABLE;
		}

		if (polling_tbl[i].task_polling == (pf_task_polling)0) {
			g_app_startup_diag.first_invalid_polling_id = polling_tbl[i].id;
			return AK_DISABLE;
		}

		if ((is_first_entry == AK_DISABLE) && (polling_tbl[i].id <= prev_id)) {
			g_app_startup_diag.first_invalid_polling_id = polling_tbl[i].id;
			return AK_DISABLE;
		}

		prev_id = polling_tbl[i].id;
		is_first_entry = AK_DISABLE;
		g_app_startup_diag.polling_task_count++;
	}

	g_app_startup_diag.first_invalid_polling_id = AK_TASK_POLLING_EOT_ID;
	return AK_DISABLE;
}

/*****************************************************************************/
/* app main function.
 */
/*****************************************************************************/
int main_app() {
	APP_PRINT("App run mode: %s, App version: %d.%d.%d.%d\n", app_run_mode, app_info.version[0] \
			, app_info.version[1]	\
			, app_info.version[2]	\
			, app_info.version[3]);

	g_app_startup_diag.task_tbl_ptr = app_task_table;
	g_app_startup_diag.polling_tbl_ptr = app_task_polling_table;
	g_app_startup_diag.app_info_valid = (app_info.magic_number == APP_MAGIC_NUMBER) ? AK_ENABLE : AK_DISABLE;
	g_app_startup_diag.task_table_valid = app_validate_task_table(app_task_table);
	g_app_startup_diag.polling_table_valid = app_validate_polling_table(app_task_polling_table);

	APP_PRINT("AK init check: app_info=%d, task_tbl=%d(count=%d), polling_tbl=%d(count=%d)\n",
			g_app_startup_diag.app_info_valid,
			g_app_startup_diag.task_table_valid,
			g_app_startup_diag.task_count,
			g_app_startup_diag.polling_table_valid,
			g_app_startup_diag.polling_task_count);

	if ((g_app_startup_diag.app_info_valid == AK_DISABLE)
			|| (g_app_startup_diag.task_table_valid == AK_DISABLE)
			|| (g_app_startup_diag.polling_table_valid == AK_DISABLE)) {
		APP_PRINT("AK init check fail: bad_task_id=%d, bad_pri=%d, bad_polling_id=%d, bad_ability=%d\n",
				g_app_startup_diag.first_invalid_task_id,
				g_app_startup_diag.first_invalid_task_pri,
				g_app_startup_diag.first_invalid_polling_id,
				g_app_startup_diag.first_invalid_polling_ability);
		return -1;
	}

	if (app_run_edp_init_probe() == AK_DISABLE) {
		APP_PRINT("EDP init probe fail: ret=%d, table_created=%d/%d, queue_removed=%d, pool_before=%lu, pool_after=%lu\n",
				g_app_startup_diag.edp_init_ret,
				g_app_startup_diag.created_task_table_size,
				g_app_startup_diag.expected_task_table_size,
				g_app_startup_diag.queue_probe_removed_count,
				(unsigned long)g_app_startup_diag.pure_pool_used_before_queue_probe,
				(unsigned long)g_app_startup_diag.pure_pool_used_after_queue_probe);
		return -2;
	}

	APP_PRINT("EDP init probe pass: ret=%d, table_created=%d/%d, queue_removed=%d, pool_before=%lu, pool_after=%lu\n",
			g_app_startup_diag.edp_init_ret,
			g_app_startup_diag.created_task_table_size,
			g_app_startup_diag.expected_task_table_size,
			g_app_startup_diag.queue_probe_removed_count,
			(unsigned long)g_app_startup_diag.pure_pool_used_before_queue_probe,
			(unsigned long)g_app_startup_diag.pure_pool_used_after_queue_probe);

	int edp_two_task_ret = app_run_edp_two_task_debug();
	if (edp_two_task_ret != 0) {
		APP_PRINT("EDP two-task debug fail: ret=%d, a_rx=%d, b_rx=%d, done=%d, deinit=%d\n",
				edp_two_task_ret,
				g_app_edp_two_task_dbg.task_a_rx_count,
				g_app_edp_two_task_dbg.task_b_rx_count,
				g_app_edp_two_task_dbg.transfer_done,
				g_app_edp_two_task_dbg.deinit_done);
		return -3;
	}

	APP_PRINT("EDP two-task debug pass: first_sig=0x%x, second_sig=0x%x, payload=%s, deinit=%d\n",
			g_app_edp_two_task_dbg.first_sig_seen,
			g_app_edp_two_task_dbg.second_sig_seen,
			g_app_edp_two_task_dbg.sample_payload,
			g_app_edp_two_task_dbg.deinit_done);

	APP_PRINT("AK runtime is bypassed: diagnostics captured only, no task init/create/run executed.\n");
	return 0;
}

/*****************************************************************************/
/* polling task can be pushed right here
 * when all ak message queue empty, task_polling_xxx() will be called.
 */
/*****************************************************************************/
void task_polling_console() {
	volatile uint8_t c = 0;

	while (ring_buffer_char_is_empty(&ring_buffer_console_rev) == false) {

		ENTRY_CRITICAL();
		c = ring_buffer_char_get(&ring_buffer_console_rev);
		EXIT_CRITICAL();

#if defined (IF_LINK_UART_EN)
		if (plink_hal_rev_byte(c) == LINK_HAL_IGNORED) {
#endif
			if (shell.index < SHELL_BUFFER_LENGHT - 1) {

				if (c == '\r' || c == '\n') { /* linefeed */

					xputc('\r');
					xputc('\n');

					shell.data[shell.index] = c;
					shell.data[shell.index + 1] = 0;
					task_post_common_msg(AC_TASK_SHELL_ID, AC_SHELL_LOGIN_CMD, (uint8_t*)&shell.data[0], shell.index + 2);

					shell.index = 0;
				}
				else {

					xputc(c);

					if (c == 8 && shell.index) { /* backspace */
						shell.index--;
					}
					else {
						shell.data[shell.index++] = c;
					}
				}
			}
			else {
				LOGIN_PRINT("\nerror: cmd too long, cmd size: %d, try again !\n", SHELL_BUFFER_LENGHT);
				shell.index = 0;
			}
#if defined (IF_LINK_UART_EN)
		}
#endif
	}
}

/*****************************************************************************/
/* app initial function.
 */
/*****************************************************************************/

/* start software timer for application
 * used for app tasks
 */
void app_start_timer() {
	/* start timer to toggle life led */
	timer_set(AC_TASK_LIFE_ID, AC_LIFE_SYSTEM_CHECK, AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, TIMER_PERIODIC);
	timer_set(AC_TASK_FW_ID, FW_CHECKING_REQ, FW_UPDATE_REQ_INTERVAL, TIMER_ONE_SHOT);
	timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_INITIAL, AC_DISPLAY_INITIAL_INTERVAL, TIMER_ONE_SHOT);
}

/* init state machine for tasks
 * used for app tasks
 */
void app_init_state_machine() {

}

/* send first message to trigger start tasks
 * used for app tasks
 */
void app_task_init() {
	SCREEN_CTOR(&scr_mng_app, scr_startup_handle, &scr_startup);
	task_post_pure_msg(AC_TASK_UART_IF_ID, AC_UART_IF_INIT);
}

/*****************************************************************************/
/* app common function
 */
/*****************************************************************************/

/* hardware timer interrupt 10ms
 * used for led, button polling
 */

void sys_irq_timer_10ms() {
}

void* app_get_boot_share_data() {
	return 0;
}
