#include "task.h"

#include <string.h>

#include "timer.h"
#include "ak.h"
#include "ak_dbg.h"
#include "message.h"
#include "timer.h"
#include "task_list.h"
#include "log_queue.h"
#include "utils.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"

typedef struct {
	task_pri_t  pri;
	uint8_t     mask;
	ak_msg_t*   qhead;
	ak_msg_t*   qtail;
} tcb_t;

static task_id_t current_task_id;
static task_t current_task_info;
static ak_msg_t current_active_object;

#if defined(AK_TASK_OBJ_LOG_ENABLE)
log_queue_t log_task_dbg_object_queue;
static uint8_t task_dbg_active_obj_queue[LOG_QUEUE_OBJECT_SIZE];
#endif

#if defined(AK_IRQ_OBJ_LOG_ENABLE)
log_queue_t log_irq_queue;
static uint8_t irq_queue[LOG_QUEUE_IRQ_SIZE];
#endif

static tcb_t	task_pri_queue[TASK_PRI_MAX_SIZE];
static task_t*	task_table = (task_t*)0;
static uint8_t	task_table_size = 0;
static uint8_t	task_current = 0;
static uint8_t	task_ready = 0;

static task_polling_t* task_polling_table = (task_polling_t*)0;
static uint8_t	task_polling_table_size = 0;

static void task_sheduler();

static void task_queue_integrity_check(tcb_t* t_tcb, uint8_t err_code) {
	if (msg_is_valid_or_null(t_tcb->qhead) == AK_MSG_NG) {
		FATAL("TK", err_code);
	}

	if (msg_is_valid_or_null(t_tcb->qtail) == AK_MSG_NG) {
		FATAL("TK", err_code);
	}

	if ((t_tcb->qhead == NULL) && (t_tcb->qtail != NULL)) {
		FATAL("TK", err_code);
	}
}

/* function MUST-BE redefine */
__AK_WEAK void task_irq_io_entry_trigger() {
#if defined(AK_IO_IRQ_ANALYZER)
	FATAL("TK", 0x03);
#endif
}

/* function MUST-BE redefine */
__AK_WEAK void task_irq_io_exit_trigger() {
#if defined(AK_IO_IRQ_ANALYZER)
	FATAL("TK", 0x04);
#endif
}

/*
 * Đăng ký bảng task theo cơ chế event-driven.
 * Bảng phải kết thúc bằng AK_TASK_EOT_ID và mỗi phần tử phải có hàm xử lý task.
 */
void task_create(task_t* task_tbl) {
	uint8_t idx = 0;
	if (task_tbl) {
		task_table = task_tbl;
		while (task_tbl[idx].id != AK_TASK_EOT_ID) {
			if (task_tbl[idx].task == (pf_task)0) {
				FATAL("TK", 0x12);
			}

			if (idx >= AK_TASK_EOT_ID) {
				FATAL("TK", 0x13);
			}

			idx++;
		}
		task_table_size = idx;
	}
	else {
		FATAL("TK", 0x01);
	}
}

/*
 * Đăng ký bảng task polling.
 * Bảng phải kết thúc bằng AK_TASK_POLLING_EOT_ID và mỗi phần tử phải có hàm polling.
 */
void task_polling_create(task_polling_t* task_polling_tbl) {
	uint8_t idx = 0;
	if (task_polling_tbl) {
		task_polling_table = task_polling_tbl;
		while (task_polling_tbl[idx].id != AK_TASK_POLLING_EOT_ID) {
			if (task_polling_tbl[idx].task_polling == (pf_task_polling)0) {
				FATAL("TK", 0x14);
			}

			if (idx >= AK_TASK_POLLING_EOT_ID) {
				FATAL("TK", 0x15);
			}

			idx++;
		}
		task_polling_table_size = idx;
	}
	else {
		FATAL("TK", 0x06);
	}
}

/*
 * Đẩy một message AK vào queue của task đích.
 * Hàm này tự cập nhật metadata nguồn/đích của message và đặt cờ task sẵn sàng.
 */
void task_post(task_id_t des_task_id, ak_msg_t* msg) {
	tcb_t* t_tcb;

	if (msg_is_valid_ptr(msg) == AK_MSG_NG) {
		FATAL("TK", 0x0D);
	}

	if (des_task_id >= task_table_size) {
		FATAL("TK", 0x02);
	}

	t_tcb = &task_pri_queue[task_table[des_task_id].pri - 1];

	ENTRY_CRITICAL();
	task_queue_integrity_check(t_tcb, 0x0E);

	msg->next = NULL;
	msg->src_task_id = current_task_id;
	msg->des_task_id = des_task_id;
	msg->if_src_task_id = msg->src_task_id;
	msg->if_des_task_id = des_task_id;

#if defined(AK_TASK_OBJ_LOG_ENABLE)
	if (get_msg_ref_count(msg) <= 1) {
		msg->dbg_handler.start_exe = 0;
		msg->dbg_handler.stop_exe = 0;
		msg->dbg_handler.start_post = sys_ctrl_millis();
	}
#endif

	if (t_tcb->qtail == NULL) {
		/* put message to queue */
		t_tcb->qtail = msg;
		t_tcb->qhead = msg;

		/* change status task to ready*/
		task_ready |= t_tcb->mask;
	}
	else {
		if (msg_is_valid_ptr(t_tcb->qtail) == AK_MSG_NG) {
			FATAL("TK", 0x0E);
		}

		if (msg_is_valid_or_null(t_tcb->qtail->next) == AK_MSG_NG) {
			FATAL("TK", 0x0E);
		}

		/* put message to queue */
		t_tcb->qtail->next = msg;
		t_tcb->qtail = msg;
	}

	EXIT_CRITICAL();
}

uint8_t task_remove_msg(task_id_t task_id, uint8_t sig) {
	tcb_t* t_tcb;
	uint8_t total_rm_msg = 0;

	ak_msg_t* del_msg = NULL; /* MUST-BE initialized NULL */
	ak_msg_t* trace_msg = NULL;
	ak_msg_t* traverse_msg;


	if (task_id >= AK_TASK_EOT_ID) {
		FATAL("TK", 0x05);
	}

	ENTRY_CRITICAL();

	/* get task table control */
	t_tcb = &task_pri_queue[task_table[task_id].pri - 1];
	task_queue_integrity_check(t_tcb, 0x0F);

	/* check task queue available */
	if (task_ready & t_tcb->mask) {

		/* get first message of queue */
		traverse_msg = t_tcb->qhead;

		while (traverse_msg != NULL) {
			if (msg_is_valid_ptr(traverse_msg) == AK_MSG_NG) {
				FATAL("TK", 0x0F);
			}

			if (msg_is_valid_or_null(traverse_msg->next) == AK_MSG_NG) {
				FATAL("TK", 0x0F);
			}

			/* check message task id and signal */
			if (traverse_msg->des_task_id == task_id && traverse_msg->sig == sig) {

				/* assign remove message */
				del_msg = traverse_msg;

				if (del_msg == t_tcb->qhead) {
					t_tcb->qhead = traverse_msg->next;
				}
				else if (del_msg != t_tcb->qhead) {
					trace_msg->next = traverse_msg->next;
				}

				/* last message of queue */
				if (del_msg->next == NULL) {
					t_tcb->qtail = trace_msg;

					/* Check if no message exist after remove current message */
					if (t_tcb->qhead == NULL) {

						/* change status of task to inactive */
						task_ready &= ~t_tcb->mask;
					}
				}
			}
			else {
				trace_msg = traverse_msg;
			}

			/* consider the next message */
			traverse_msg = traverse_msg->next;

			/* free the message if it's found */
			if (del_msg != NULL) {
				msg_force_free(del_msg);
				del_msg = NULL;
				total_rm_msg++;
			}
		}
	}

	EXIT_CRITICAL();
	return total_rm_msg;
}

void task_post_pure_msg(task_id_t des_task_id, uint8_t sig) {
	ak_msg_t* s_msg = get_pure_msg();
	set_msg_sig(s_msg, sig);
	task_post(des_task_id, s_msg);
}

void task_post_common_msg(task_id_t des_task_id, uint8_t sig, uint8_t* data, uint8_t len) {
	ak_msg_t* s_msg = get_common_msg();
	set_msg_sig(s_msg, sig);
	set_data_common_msg(s_msg, data, len);
	task_post(des_task_id, s_msg);
}

void task_post_dynamic_msg(task_id_t des_task_id, uint8_t sig, uint8_t* data, uint32_t len) {
	ak_msg_t* s_msg = get_dynamic_msg();
	set_msg_sig(s_msg, sig);
	set_data_dynamic_msg(s_msg, data, len);
	task_post(des_task_id, s_msg);
}

void task_entry_interrupt() {

	ENTRY_CRITICAL();

	task_irq_io_entry_trigger();
	current_task_id = AK_TASK_INTERRUPT_ID;

#if defined(AK_IRQ_OBJ_LOG_ENABLE)
	exception_info_t exception_info;
	exception_info.except_number = sys_ctr_get_exception_number();
	exception_info.timestamp = sys_ctrl_millis();

	log_queue_put(&log_irq_queue, &exception_info);
#endif

	EXIT_CRITICAL();
}

void task_exit_interrupt() {

	ENTRY_CRITICAL();

	current_task_id = current_task_info.id;
	task_irq_io_exit_trigger();

	EXIT_CRITICAL();
}

int task_init() {
	uint8_t pri;
	tcb_t* t_tcb;

	/*
	 * Reset trạng thái scheduler và toàn bộ queue theo từng mức ưu tiên
	 * trước khi chạy runtime AK.
	 * Bắt buộc gọi trước task_create()/task_polling_create()/task_run().
	 */
	/* init task manager variable */
	current_task_id = 0;
	memset(&current_task_info, 0, sizeof(task_t));
	memset(&current_active_object, 0, sizeof(ak_msg_t));
	task_current = 0;
	task_ready = 0;

	/* init kernel queue */
	for (pri = 1; pri <= TASK_PRI_MAX_SIZE; pri++) {
		t_tcb = &task_pri_queue[pri - 1];
		t_tcb->mask     = (1 << (pri - 1));
		t_tcb->qhead    = NULL;
		t_tcb->qtail    = NULL;
		task_queue_integrity_check(t_tcb, 0x10);
	}

	/* message manager must be initial fist */
	msg_init();

	/* init timer manager */
	timer_init();

	return 0;
}

/*
 * Vòng lặp runtime chính của AK.
 * Luôn ưu tiên chạy task có mức ưu tiên cao nhất đang sẵn sàng,
 * sau đó mới chạy nhóm task polling.
 */
int task_run() {
	/* init active object log queue */
#if defined(AK_TASK_OBJ_LOG_ENABLE)
	log_queue_init(&log_task_dbg_object_queue \
				   , (uint32_t)task_dbg_active_obj_queue \
				   , (LOG_QUEUE_OBJECT_SIZE / sizeof(ak_msg_t)) \
				   , sizeof(ak_msg_t) \
				   , mem_write \
				   , mem_read);
#endif

	/* init irq log queue */
#if defined(AK_IRQ_OBJ_LOG_ENABLE)
	log_queue_init(&log_irq_queue \
				   , (uint32_t)irq_queue \
				   , (LOG_QUEUE_IRQ_SIZE / sizeof(exception_info_t)) \
				   , sizeof(exception_info_t) \
				   , mem_write \
				   , mem_read);
#endif

	SYS_PRINT("[task_run] Active Objects is ready\n\n");

	for (;;) {
		task_sheduler();
		task_polling_run();
	}
}

/*
 * Chạy một vòng scheduler phục vụ debug mà không đi vào vòng lặp vô hạn.
 * Trả về 1 nếu có task sẵn sàng để xử lý, trả về 0 nếu không có task nào trong queue.
 */
int task_debug_run_once() {
	ENTRY_CRITICAL();

	if (task_ready == 0) {
		EXIT_CRITICAL();
		return 0;
	}

	EXIT_CRITICAL();

	task_sheduler();
	task_polling_run();

	return 1;
}

int task_sample_run_once() {
	ENTRY_CRITICAL();

	if (task_ready == 0) {
		EXIT_CRITICAL();
		return 0;
	}

	EXIT_CRITICAL();

	task_sheduler();

	return 1;
}

/*
 * Deinit task manager cho mục đích test/debug:
 * - xóa liên kết bảng task/polling,
 * - reset queue và trạng thái scheduler,
 * - reset message/timer pool về trạng thái mới khởi tạo.
 */
void task_deinit() {
	uint8_t pri;
	tcb_t* t_tcb;

	ENTRY_CRITICAL();

	current_task_id = 0;
	memset(&current_task_info, 0, sizeof(task_t));
	memset(&current_active_object, 0, sizeof(ak_msg_t));

	task_table = (task_t*)0;
	task_table_size = 0;
	task_current = 0;
	task_ready = 0;

	task_polling_table = (task_polling_t*)0;
	task_polling_table_size = 0;

	for (pri = 1; pri <= TASK_PRI_MAX_SIZE; pri++) {
		t_tcb = &task_pri_queue[pri - 1];
		t_tcb->mask = (1 << (pri - 1));
		t_tcb->qhead = NULL;
		t_tcb->qtail = NULL;
	}

	EXIT_CRITICAL();

	msg_init();
	timer_init();
}

void task_polling_set_ability(task_id_t task_polling_id, uint8_t ability) {
	task_polling_t* __task_polling_table = task_polling_table;

	while (__task_polling_table->id < AK_TASK_POLLING_EOT_ID) {

		if (__task_polling_table->id == task_polling_id) {

			ENTRY_CRITICAL();

			__task_polling_table->ability = ability;

			EXIT_CRITICAL();

			break;
		}

		__task_polling_table++;
	}

	if (__task_polling_table->id == AK_TASK_POLLING_EOT_ID) {
		FATAL("TK", 0x07);
	}
}

void task_polling_run() {
	task_polling_t* __task_polling_table = task_polling_table;

	while (__task_polling_table->id < AK_TASK_POLLING_EOT_ID) {

		ENTRY_CRITICAL();
		if (__task_polling_table->ability == AK_ENABLE) {

			EXIT_CRITICAL();
			__task_polling_table->task_polling();
		}
		else {
			EXIT_CRITICAL();
		}
		__task_polling_table++;
	}
}

void task_sheduler() {
	uint8_t t_task_new;

	ENTRY_CRITICAL();

	uint8_t t_task_current = task_current;

	/* task_ready phải có ít nhất một bit được set trước khi gọi LOG2LKUP(). */
	while ((task_ready != 0) && ((t_task_new = LOG2LKUP(task_ready)) > t_task_current)) {
		/* get task */
		tcb_t* t_tcb = &task_pri_queue[t_task_new];
		task_queue_integrity_check(t_tcb, 0x11);

		if (t_tcb->qhead == NULL) {
			FATAL("TK", 0x11);
		}

		/* get message */
		ak_msg_t* t_msg = t_tcb->qhead;
		if (msg_is_valid_ptr(t_msg) == AK_MSG_NG) {
			FATAL("TK", 0x11);
		}

		if (msg_is_valid_or_null(t_msg->next) == AK_MSG_NG) {
			FATAL("TK", 0x11);
		}
		t_tcb->qhead = t_msg->next;

		/* last message of queue */
		if (t_msg->next == NULL) {
			t_tcb->qtail = NULL;
			/* change status of task to inactive */
			task_ready &= ~t_tcb->mask;
		}

		/* update current task */
		task_current = t_task_new;

		/* start task debug */
#if defined(AK_TASK_OBJ_LOG_ENABLE) || defined(AK_TASK_LOG_CONSOLE_ENABLE)
		t_msg->dbg_handler.start_exe = sys_ctrl_millis();
#endif
		if (t_msg->des_task_id >= task_table_size) {
			FATAL("TK", 0x09);
		}

		if (task_table[t_msg->des_task_id].task == (pf_task)0) {
			FATAL("TK", 0x0A);
		}

		/* update current ak object */
		memcpy(&current_task_info, &task_table[t_msg->des_task_id], sizeof(task_t));
		memcpy(&current_active_object, t_msg, sizeof(ak_msg_t));

		/* update current task id by local scheduler destination */
		current_task_id = t_msg->des_task_id;

		/* execute task */
		EXIT_CRITICAL();

		pf_task task_handler = task_table[t_msg->des_task_id].task;
		if ((((uint32_t)task_handler) & 0x01U) == 0U) {
			FATAL("TK", 0x08);
		}

		task_handler(t_msg);

		ENTRY_CRITICAL();

#if defined(AK_TASK_OBJ_LOG_ENABLE) || defined(AK_TASK_LOG_CONSOLE_ENABLE)
		/* reject msg of timer task */
		if (current_active_object.des_task_id > 0) {
			current_active_object.dbg_handler.stop_exe = sys_ctrl_millis();

			/* put current object to log queue */
			log_queue_put(&log_task_dbg_object_queue, &current_active_object);

#if defined(AK_TASK_LOG_CONSOLE_ENABLE)
			{
				uint32_t exe_time;
				uint32_t wait_time;
				if (current_active_object.dbg_handler.start_exe >= current_active_object.dbg_handler.start_post) {
					wait_time = current_active_object.dbg_handler.start_exe - current_active_object.dbg_handler.start_post;
				}
				else {
					wait_time = current_active_object.dbg_handler.start_exe + ((uint32_t)0xFFFFFFFF - current_active_object.dbg_handler.start_post);
				}

				if (current_active_object.dbg_handler.stop_exe >= current_active_object.dbg_handler.start_exe) {
					exe_time = current_active_object.dbg_handler.stop_exe - current_active_object.dbg_handler.start_exe;
				}
				else {
					exe_time = current_active_object.dbg_handler.stop_exe + ((uint32_t)0xFFFFFFFF - current_active_object.dbg_handler.start_exe);
				}

				xprintf("taskID: %d\tmsgType:0x%x\trefCnt:%d\tsig:%d\t\twaitTime:%d\texeTime:%d\n"\
						, current_active_object.des_task_id								\
						, (current_active_object.ref_count & AK_MSG_TYPE_MASK)		\
						, (current_active_object.ref_count & AK_MSG_REF_COUNT_MASK)	\
						, current_active_object.sig									\
						, (wait_time)	\
						, (exe_time));
			}
#endif
		}

		if (get_msg_ref_count(t_msg) > 1) {
			t_msg->dbg_handler.start_post = current_active_object.dbg_handler.stop_exe ;
			t_msg->dbg_handler.start_exe = 0;
			t_msg->dbg_handler.stop_exe = 0;
		}

		/* clear current active object */
		memset(&current_active_object, 0, sizeof(ak_msg_t));

#endif

		/* check and free message */
		msg_free(t_msg);
	}

	task_current = t_task_current;

	current_task_id = AK_TASK_IDLE_ID;

	EXIT_CRITICAL();
}

void task_pri_queue_dump() {
	uint8_t t_task_ready;
	tcb_t* t_tcb;
	ak_msg_t* t_msg;

	ENTRY_CRITICAL();

	t_task_ready = task_ready;

	for (uint8_t pri = 1; pri <= TASK_PRI_MAX_SIZE; pri++) {
		t_tcb = &task_pri_queue[pri - 1];

		/* check task queue available */
		if (t_task_ready & t_tcb->mask) {

			/* get first message of queue */
			t_msg = t_tcb->qhead;

			while (t_msg != NULL) {

				/* dump message queue */
				xprintf("srcTaskID:%d\tdesTaskID:%d\tmsgType:0x%x\trefCnt:%d\tsig:%d\n"\
						, t_msg->src_task_id \
						, t_msg->des_task_id \
						, (t_msg->ref_count & AK_MSG_TYPE_MASK) \
						, (t_msg->ref_count & AK_MSG_REF_COUNT_MASK) \
						, t_msg->sig);

				/* consider the next message */
				t_msg = t_msg->next;
			}
		}
	}

	EXIT_CRITICAL();
}

task_id_t task_self() {
	return current_task_info.id;
}

task_id_t get_current_task_id() {
	return current_task_id;
}

task_t* get_current_task_info() {
	return (task_t*)&current_task_info;
}

ak_msg_t* get_current_active_object() {
	return (ak_msg_t*)&current_active_object;
}

uint8_t get_task_table_size() {
	return task_table_size;
}


