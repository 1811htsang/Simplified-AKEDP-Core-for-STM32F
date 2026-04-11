#include "task_list.h"

#include "timer.h"

const task_t app_task_table[] = {
	/*************************************************************************/
	/* SYSTEM TASK */
	/*************************************************************************/
	{TASK_TIMER_TICK_ID,		TASK_PRI_LEVEL_7,		task_timer_tick			},

	/*************************************************************************/
	/* APP TASK */
	/*************************************************************************/
	{AC_TASK_SYSTEM_ID			,	TASK_PRI_LEVEL_2	,	task_system			},
	{AC_TASK_IF_ID				,	TASK_PRI_LEVEL_4	,	task_if				},

	/*************************************************************************/
	/* END OF TABLE */
	/*************************************************************************/
	{AK_TASK_EOT_ID				,	TASK_PRI_LEVEL_0	,	(pf_task)0			}
};

const task_polling_t app_task_polling_table[] = {
	{AC_TASK_POLLING_CONSOLE_ID	,	AK_ENABLE	,	task_polling_console		},
	{AK_TASK_POLLING_EOT_ID		,	AK_DISABLE	,	(pf_task_polling)0			},
};

