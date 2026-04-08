#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define FLASH_PAGE_SIZE 0x400U
#define FLASH_BLOCK_64K_SIZE 0x10000U

void flash_read(uint32_t address, uint8_t* data, uint32_t length);
void flash_write(uint32_t address, uint8_t* data, uint32_t length);
void flash_erase_sector(uint32_t address);
void flash_erase_block_64k(uint32_t address);

void internal_flash_unlock(void);
void internal_flash_lock(void);
void internal_flash_erase_pages_cal(uint32_t address, uint32_t length);
void internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif