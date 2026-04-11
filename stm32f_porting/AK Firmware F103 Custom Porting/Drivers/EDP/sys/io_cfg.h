#ifndef __IO_CFG_H__
#define __IO_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "main.h"

/* Internal flash helpers used by firmware update flow. */
void internal_flash_unlock(void);
void internal_flash_lock(void);
void internal_flash_erase_pages_cal(uint32_t address, uint32_t length);
void internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
