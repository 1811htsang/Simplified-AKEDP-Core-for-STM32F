#include "task_uart_if.h"

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"
#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "xprintf.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "sys_io.h"
#include "sys_irq.h"

#if defined (IF_LINK_UART_EN)
#include "link_sig.h"
#include "link.h"
#endif

/* Bridge UART interface signals to the link layer when UART linking is enabled. */
void task_uart_if(ak_msg_t* msg) {
	switch (msg->sig) {
#if defined (IF_LINK_UART_EN)
	case AC_UART_IF_INIT: {
		APP_DBG_SIG("AC_UART_IF_INIT\n");
		link_init_state_machine();
		task_post_pure_msg(AC_LINK_ID, AC_LINK_INIT);
	}
		break;

	case AC_UART_IF_PURE_MSG_OUT: {
		APP_DBG_SIG("AC_UART_IF_PURE_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_LINK_SEND_PURE_MSG);
		task_post(AC_LINK_ID, msg);
	}
		break;

	case AC_UART_IF_COMMON_MSG_OUT: {
		APP_DBG_SIG("AC_UART_IF_COMMON_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_LINK_SEND_COMMON_MSG);
		task_post(AC_LINK_ID, msg);
	}
		break;

	case AC_UART_IF_DYNAMIC_MSG_OUT: {
		APP_DBG_SIG("AC_UART_IF_DYNAMIC_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_LINK_SEND_DYNAMIC_MSG);
		task_post(AC_LINK_ID, msg);
	}
		break;

	case AC_UART_IF_PURE_MSG_IN: {
		APP_DBG_SIG("AC_UART_IF_PURE_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_IF_PURE_MSG_IN);
		task_post(AC_TASK_IF_ID, msg);
	}
		break;

	case AC_UART_IF_COMMON_MSG_IN: {
		APP_DBG_SIG("AC_UART_IF_COMMON_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_IF_COMMON_MSG_IN);
		task_post(AC_TASK_IF_ID, msg);
	}
		break;

	case AC_UART_IF_DYNAMIC_MSG_IN: {
		APP_DBG_SIG("AC_UART_IF_DYNAMIC_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, AC_IF_DYNAMIC_MSG_IN);
		task_post(AC_TASK_IF_ID, msg);
	}
		break;
#endif
	default:
		break;
	}
}

