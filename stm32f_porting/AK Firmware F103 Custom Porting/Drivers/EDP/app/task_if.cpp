#include "task_if.h"

#include <stdbool.h>
#include <stdint.h>

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"
#include "app.h"
#include "app_data.h"
#include "app_dbg.h"
#include "app_if.h"
#include "task_list.h"
#include "sys_dbg.h"
#include "sys_io.h"
#include "sys_irq.h"

/* Forward only UART-addressed interface messages into the active core path. */
static void if_des_type_uart_handler(ak_msg_t* msg);

/* Dispatch interface messages based on destination type. */
void task_if(ak_msg_t* msg) {
	/* UART */
	if (msg->if_des_type >= IF_TYPE_UART_GW_MIN &&
			 msg->if_des_type <= IF_TYPE_UART_GW_MAX) {
		if_des_type_uart_handler(msg);
	}
}

/* Translate UART interface message signals into core task messages. */
void if_des_type_uart_handler(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_IF_PURE_MSG_IN: {
		APP_DBG_SIG("AC_IF_PURE_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case AC_IF_COMMON_MSG_IN: {
		APP_DBG_SIG("AC_IF_COMMON_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case AC_IF_DYNAMIC_MSG_IN: {
		APP_DBG_SIG("AC_IF_DYNAMIC_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case AC_IF_PURE_MSG_OUT: {
		APP_DBG_SIG("AC_IF_PURE_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_UART_IF_PURE_MSG_OUT);
		task_post(AC_TASK_UART_IF_ID, msg);
	}
		break;

	case AC_IF_COMMON_MSG_OUT: {
		APP_DBG_SIG("AC_IF_COMMON_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_UART_IF_COMMON_MSG_OUT);
		task_post(AC_TASK_UART_IF_ID, msg);
	}
		break;

	case AC_IF_DYNAMIC_MSG_OUT: {
		APP_DBG_SIG("AC_IF_DYNAMIC_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_UART_IF_DYNAMIC_MSG_OUT);
		task_post(AC_TASK_UART_IF_ID, msg);
	}
		break;

	default:
		break;
	}
}

