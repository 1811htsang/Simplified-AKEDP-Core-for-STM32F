// Khai báo thư viện sử dụng
#include <stdint.h>
#include "app.h"
#include "ak.h"
#include "fsm.h"
#include "message.h"
#include "timer.h"
#include "task_if.h"
#include "task_list.h"
#include "sys_ctrl.h"
#include "io_cfg.h"

// Khai báo directive build
#if defined(RELEASE)
	const char* app_run_mode = "RELEASE";
#else
	// static const char* app_run_mode = "DEBUG";
#endif

const app_info_t app_info { \
	APP_MAGIC_NUMBER, \
	APP_VER, \
};

static uint8_t app_run_edp_init_probe();
static int app_run_edp_two_task_debug();
static void app_task_edp_dbg_a(ak_msg_t* msg);
static void app_task_edp_dbg_b(ak_msg_t* msg);

#define APP_EDP_PROBE_SIG (0xF1u)
#define APP_EDP_DBG_SIG_START (0xA1u)
#define APP_EDP_DBG_SIG_FORWARD (0xB1u)

typedef enum {
	APP_EDP_DBG_TASK_A_ID = 0,
	APP_EDP_DBG_TASK_B_ID,
	APP_EDP_DBG_TASK_EOT_ID
} app_edp_dbg_task_id_t;

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
	unsigned long pure_pool_used_before_queue_probe;
	unsigned long pure_pool_used_after_queue_probe;
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

static int app_run_edp_two_task_debug() {
	memset((void*)&g_app_edp_two_task_dbg, 0, sizeof(app_edp_two_task_dbg_t));

	if (task_init() != 0) {
		return -10;
	}
	g_app_edp_two_task_dbg.init_ret = AK_ENABLE;

	task_create((task_t*)app_edp_dbg_task_table);
	task_polling_create((task_polling_t*)app_edp_dbg_polling_table);
	g_app_edp_two_task_dbg.created_ret = AK_ENABLE;

	task_post_pure_msg(APP_EDP_DBG_TASK_A_ID, APP_EDP_DBG_SIG_START);

	g_app_edp_two_task_dbg.first_dispatch_ret = (uint8_t)task_debug_run_once();

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
	g_app_startup_diag.task_tbl_ptr = app_task_table;
	g_app_startup_diag.polling_tbl_ptr = app_task_polling_table;
	g_app_startup_diag.app_info_valid = (app_info.magic_number == APP_MAGIC_NUMBER) ? AK_ENABLE : AK_DISABLE;
	g_app_startup_diag.task_table_valid = app_validate_task_table(app_task_table);
	g_app_startup_diag.polling_table_valid = app_validate_polling_table(app_task_polling_table);

	if ((g_app_startup_diag.app_info_valid == AK_DISABLE)
			|| (g_app_startup_diag.task_table_valid == AK_DISABLE)
			|| (g_app_startup_diag.polling_table_valid == AK_DISABLE)) {
		return -1;
	}

	if (app_run_edp_init_probe() == AK_DISABLE) {
		return -2;
	}

	int edp_two_task_ret = app_run_edp_two_task_debug();
	if (edp_two_task_ret != 0) {
	}

	return 0;
}

/*****************************************************************************/
/* polling task can be pushed right here
 * when all ak message queue empty, task_polling_xxx() will be called.
 */
/*****************************************************************************/
void task_polling_console() {
	/* Shell is detached from minimalist AK-EDP build; keep polling hook as no-op. */
}

/*****************************************************************************/
/* app initial function.
 */
/*****************************************************************************/


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

