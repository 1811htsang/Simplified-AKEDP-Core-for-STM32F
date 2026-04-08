#ifndef __APP_DATA_H__
#define __APP_DATA_H__

#include <stdint.h>

/******************************************************************************
*  IF type ranges used by the active core paths.
*******************************************************************************/
/** APP interface, communication via socket interface
 *
 */
#define IF_TYPE_APP_MIN						(100)
#define IF_TYPE_APP_GW						(100)
#define IF_TYPE_APP_GI						(101)
#define IF_TYPE_APP_MAX						(119)

/** UART interface
 *
 */
#define IF_TYPE_UART_GW_MIN					(120)
#define IF_TYPE_UART_GW						(120)
#define IF_TYPE_UART_AC						(121)
#define IF_TYPE_UART_GW_MAX					(140)


/* Shared boot-state snapshot passed between bootloader and app runtime. */
typedef struct {
	uint8_t is_power_on_reset;
} boot_app_share_data_t;

#endif //__APP_DATA_H__
