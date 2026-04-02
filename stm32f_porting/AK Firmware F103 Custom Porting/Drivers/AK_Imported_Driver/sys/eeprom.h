#ifndef __EEPROM_H__
#define __EEPROM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void eeprom_read(uint32_t address, uint8_t* data, uint32_t length);
void eeprom_write(uint32_t address, const uint8_t* data, uint32_t length);
void eeprom_erase(uint32_t address, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif