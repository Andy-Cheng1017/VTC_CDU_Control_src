#ifndef __RS485_REGION_H_
#define __RS485_REGION_H_
#include <stdint.h>
#include <stdbool.h>
#include "at32f403a_407.h"
#include "RS485.h"
#include "sensor_task.h"
#include "pump_task.h"

#define MY_485_ADDR 0x21

uint32_t SysStat_Handler(rs485_func_t func, uint16_t addr, uint16_t data, uint8_t len);
uint32_t DataRead_Handler(rs485_func_t func, uint16_t addr, uint16_t data, uint8_t len);
uint32_t DevCtrl_Handler(rs485_func_t func, uint16_t addr, uint16_t data, uint8_t len);
uint32_t EthConfig_Handler(rs485_func_t func, uint16_t addr, uint16_t data, uint8_t len);
uint32_t SensCard_Handler(rs485_func_t func, uint16_t addr, uint16_t data, uint8_t len);

#endif  // __RS485_REGION_H_