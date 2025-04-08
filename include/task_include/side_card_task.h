#ifndef SIDE_CARD_TASK_H
#define SIDE_CARD_TASK_H

#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#define SENS_CARD_ADDR 0x22
#define FANS_CARD_ADDR 0x23

#define CARD_DATA_MAX_SIZE 32

// #define SENS_CARD_REG_START 0x0070
// #define SENS_CARD_REG_END 0x007F
// #define SENS_CARD_TOTAL_REG_NUM (SENS_CARD_REG_END - SENS_CARD_REG_START + 1)


// #define FANS_CARD_REG_START 0x0100
// #define FANS_CARD_REG_END 0x011f
// #define FANS_CARD_TOTAL_REG_NUM (FANS_CARD_REG_END - FANS_CARD_REG_START + 1)
// #define FANS_CARD_WRITE_REG_START 0x0110
// #define FANS_CARD_WRITE_REG_END 0x011F
// #define FANS_CARD_WRITE_NUM (FANS_CARD_WRITE_REG_END - FANS_CARD_WRITE_REG_START + 1)

#define READ_CARD_TASK_PRIO 2
#define READ_CARD_STK_SIZE 512

#define RS485_READ_TIMEOUT 50
#define RS485_SIDECARD_READ_PERIOD 500

extern TaskHandle_t SideCardHandler;
extern TaskHandle_t ReadCardHandler;

typedef struct {
  int32_t pt100_1_temp_m; 
  int32_t pt100_2_temp_m;  
  int32_t pt100_3_temp_m;  
  int32_t pt100_4_temp_m;    
  int16_t press_1_val_kpa;
  int16_t press_2_val_kpa;
  uint8_t leak_sensor;
  int16_t temperature;    
  uint16_t humidity;      
} SensCardStat_t;

extern SensCardStat_t SensCardStat;

typedef struct {
  uint16_t pressure_pump;
}SensCardCtrl_t;

extern SensCardCtrl_t SensCardCtrl;

typedef struct {
  uint16_t fan_pwm[16];
} FansCardCtrl_t;

extern FansCardCtrl_t FansCardCtrl;

typedef struct {
  uint16_t fan_fg[16];  // 0x0080
} FansCardStat_t;

extern FansCardStat_t FansCardStat;

void SideCardTaskFunc(void* pvParameters);
void ReadCardTaskFunc(void* pvParameters);

#endif  // SIDE_CARD_TASK_H