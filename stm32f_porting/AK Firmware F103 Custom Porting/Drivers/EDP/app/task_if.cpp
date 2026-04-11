#include "task_if.h"

#include <stdbool.h>
#include <stdint.h>

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"
#include "app.h"
#include "app_if.h"
#include "task_list.h"
#include "io_cfg.h"

// Forward declaration of handler for messages with destination type to post to other tasks
static void if_des_type_handler(ak_msg_t* msg);

/* Dispatch interface messages based on destination type. */
void task_if(ak_msg_t* msg) {
	if_des_type_handler(msg);
}

// Handle messages with destination type to post to other tasks
void if_des_type_handler(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_IF_PURE_MSG_IN: {
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case AC_IF_COMMON_MSG_IN: {
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case AC_IF_DYNAMIC_MSG_IN: {
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case AC_IF_PURE_MSG_OUT: {
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_IF_PURE_MSG_OUT);
		task_post(AC_TASK_IF_ID, msg);
	}
		break;

	case AC_IF_COMMON_MSG_OUT: {
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_IF_COMMON_MSG_OUT);
		task_post(AC_TASK_IF_ID, msg);
	}
		break;

	case AC_IF_DYNAMIC_MSG_OUT: {
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_IF_DYNAMIC_MSG_OUT);
		task_post(AC_TASK_IF_ID, msg);
	}
		break;

	default:
		break;
	}
}

