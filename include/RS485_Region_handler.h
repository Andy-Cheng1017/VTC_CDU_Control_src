#ifndef __RS485_H_
#define __RS485_H_
#include <stdint.h>
#include <stdbool.h>
#include "at32f403a_407.h"

void SysStat_Handler(uint8_t func, uint16_t addr, uint8_t *data, int len);
void DataRead_Handler(uint8_t func, uint16_t addr, uint8_t *data, int len);
void DevCtrl_Handler(uint8_t func, uint16_t addr, uint8_t *data, int len);
void EthConfig_Handler(uint8_t func, uint16_t addr, uint8_t *data, int len);

#endif  // __RS485_H_