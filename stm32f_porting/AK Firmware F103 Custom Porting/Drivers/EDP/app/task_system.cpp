#include "task_system.h"

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "app.h"
#include "app_dbg.h"
#include "app_flash.h"
#include "task_list.h"
#include "sys_boot.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"

void task_system(ak_msg_t* msg) {
	switch (msg->sig) {

	default:
		break;
	}
}

