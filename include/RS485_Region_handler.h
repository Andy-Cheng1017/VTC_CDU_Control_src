#ifndef __RS485_REGION_H_
#define __RS485_REGION_H_
#include <stdint.h>
#include <stdbool.h>
#include "RS485_enum.h"

#define MY_485_ADDR 0x21

#define SYSINFOM_REG_START 0x0000
#define SYSINFOM_REG_END 0x000F
#define SYSINFOM_TOTAL_REG_NUM (SYSINFOM_REG_END - SYSINFOM_REG_START + 1)

#define SYSPARASET_REG_START 0x0010
#define SYSPARASET_REG_END 0x003F
#define SYSPARASET_TOTAL_REG_NUM (SYSPARASET_REG_END - SYSPARASET_REG_START + 1)

#define SYSPARADISP_REG_START 0x0040
#define SYSPARADISP_REG_END 0x004F
#define SYSPARADISP_TOTAL_REG_NUM (SYSPARADISP_REG_END - SYSPARADISP_REG_START + 1)

#define DATAREAD_REG_START 0x0050
#define DATAREAD_REG_END 0x006F
#define DATAREAD_TOTAL_REG_NUM (DATAREAD_REG_END - DATAREAD_REG_START + 1)

#define SENS_CARD_DATAREAD_REG_START 0x0070
#define SENS_CARD_REG_END 0x007F
#define SENS_CARD_TOTAL_REG_NUM (SENS_CARD_REG_END - SENS_CARD_DATAREAD_REG_START + 1)

#define DEVCTRL_REG_START 0x0080
#define DEVCTRL_REG_END 0x008F
#define DEVCTRL_TOTAL_REG_NUM (DEVCTRL_REG_END - DEVCTRL_REG_START + 1)

#define SENS_CARD_DEVCTRL_REG_START 0x0090
#define SENS_CARD_DEVCTRL_REG_END 0x009F
#define SENS_CARD_DEVCTRL_TOTAL_REG_NUM (SENS_CARD_DEVCTRL_REG_END - SENS_CARD_DEVCTRL_REG_START + 1)

#define FANS_CARD_REG_START 0x00A0
#define FANS_CARD_REG_END 0x00BF
#define FANS_CARD_TOTAL_REG_NUM (FANS_CARD_REG_END - FANS_CARD_REG_START + 1)

#define FANS_CARD_WRITE_REG_START 0x00B0
#define FANS_CARD_WRITE_REG_END 0x00BF
#define FANS_CARD_WRITE_NUM (FANS_CARD_WRITE_REG_END - FANS_CARD_WRITE_REG_START + 1)

#define FANS_CARD_SYS_SET_REG_START 0x00D0
#define FANS_CARD_SYS_SET_REG_END 0x00DF
#define FANS_CARD_SYS_SET_NUM (FANS_CARD_SYS_SET_REG_END - FANS_CARD_SYS_SET_REG_START + 1)

#define FANS_CARD_SYS_DISP_REG_START 0x00E0
#define FANS_CARD_SYS_DISP_REG_END 0x00EF
#define FANS_CARD_SYS_DISP_NUM (FANS_CARD_SYS_DISP_REG_END - FANS_CARD_SYS_DISP_REG_START + 1)

uint32_t SysInfom_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t SysParaSet_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t SysParaDisp_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t SideCar_Sens_DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t SideCar_Sens_DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t FansCard_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t FanCardSysSet_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);
uint32_t FanCardSysDisp_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root);

#endif  // __RS485_REGION_H_