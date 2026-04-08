/**
 ******************************************************************************
 * @author: GaoKong
 * @date:   30/1/2017
 * @brief:  app external signal define
 ******************************************************************************
**/
#ifndef __APP_IF_H__
#define __APP_IF_H__
#include <stdint.h>
#include "ak.h"

/*****************************************************************************/
/* task GW_IF define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
enum {
	GW_CPU_SERIAL_IF_PURE_MSG_IN = AK_USER_DEFINE_SIG,
	GW_CPU_SERIAL_IF_PURE_MSG_OUT,
	GW_CPU_SERIAL_IF_COMMON_MSG_IN,
	GW_CPU_SERIAL_IF_COMMON_MSG_OUT,
	GW_CPU_SERIAL_IF_DYNAMIC_MSG_IN,
	GW_CPU_SERIAL_IF_DYNAMIC_MSG_OUT,
};

/*****************************************************************************/
/*  task GW_CONSOLE define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
enum {
	GW_CONSOLE_INTERNAL_LOGIN_CMD = AK_USER_DEFINE_SIG,
};

/*****************************************************************************/
/* task GW_IF define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
enum {
	GW_IF_PURE_MSG_IN = AK_USER_DEFINE_SIG,
	GW_IF_PURE_MSG_OUT,
	GW_IF_COMMON_MSG_IN,
	GW_IF_COMMON_MSG_OUT,
	GW_IF_DYNAMIC_MSG_IN,
	GW_IF_DYNAMIC_MSG_OUT,
};

/*****************************************************************************/
/*  task GW_TASK_DEBUG_MSG define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
enum {
	GW_DEBUG_1 = AK_USER_DEFINE_SIG,
};


#endif //__APP_IF_H__

