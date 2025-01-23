#ifndef SIDE_CARD_TASK_H
#define SIDE_CARD_TASK_H

#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

#define SENS_CARD_ADDR 0x22
#define FANS_CARD_ADDR 0x23

#define CARD_DATA_MAX_SIZE 32

#define SENS_CARD_REG_START 0x0010
#define SENS_CARD_TOTLA_REG_NUM 7
#define FANS_CARD_REG_START 0x0010
#define FANS_CARD_TOTLA_REG_NUM 16

#define SENS_TASK_PRIO 3
#define SENS_STK_SIZE 512
#define FANS_TASK_PRIO 4
#define FANS_STK_SIZE 512

extern TaskHandle_t SideCardHandler;
extern TaskHandle_t SensCardHandler;
extern TaskHandle_t FansHandler;

typedef struct {
  int16_t pt100_1_temp_m;  // 0x0010
  int16_t pt100_2_temp_m;  // 0x0011
  uint16_t AI1;          // 0x0012
  uint16_t AI2;          // 0x0013
  int16_t Temperture;    // 0x0014
  uint16_t Humidity;     // 0x0015
  bool DI1;              // 0x0016
  bool DI2;
  bool DI3;
  bool DI4;
} SensCardStat_t;

extern SensCardStat_t SensCardStat;

typedef struct {
  uint16_t fans_speed;
} FansCardCtrl_t;

extern FansCardCtrl_t FansCardCtrl;

typedef struct {
  uint16_t fan1_FB;  // 0x0010
  uint16_t fan2_FB;
  uint16_t fan3_FB;
  uint16_t fan4_FB;
  uint16_t fan5_FB;
  uint16_t fan6_FB;
  uint16_t fan7_FB;
  uint16_t fan8_FB;
  uint16_t fan9_FB;
  uint16_t fan10_FB;
  uint16_t fan11_FB;
  uint16_t fan12_FB;
  uint16_t fan13_FB;
  uint16_t fan14_FB;
  uint16_t fan15_FB;
  uint16_t fan16_FB;  // 0x001F
} FansCardStat_t;

extern FansCardStat_t FansCardStat;

void SideCardTaskFunc(void* pvParameters);
void SensCardTaskFunc(void* pvParameters);
void FansCardTaskFunc(void* pvParameters);

#endif  // SIDE_CARD_TASK_H