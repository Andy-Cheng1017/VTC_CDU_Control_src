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

#define SENS_CARD_REG_START 0x0030
#define SENS_CARD_REG_END 0x005F
#define SENS_CARD_TOTAL_REG_NUM (SENS_CARD_REG_END - SENS_CARD_REG_START + 1)


#define FANS_CARD_REG_START 0x0080
#define FANS_CARD_REG_END 0x009F
#define FANS_CARD_TOTAL_REG_NUM (FANS_CARD_REG_END - FANS_CARD_REG_START + 1)
#define FANS_CARD_WRITE_REG_START 0x0090
#define FANS_CARD_WRITE_REG_END 0x009F
#define FANS_CARD_WRITE_NUM (FANS_CARD_WRITE_REG_END - FANS_CARD_WRITE_REG_START + 1)

#define READ_CARD_TASK_PRIO 2
#define READ_CARD_STK_SIZE 512

#define RS485_READ_TIMEOUT 500
#define RS485_SIDECARD_READ_PERIOD 1000

extern TaskHandle_t SideCardHandler;
extern TaskHandle_t ReadCardHandler;

typedef struct {
  int32_t pt100_1_temp_m; 
  int32_t pt100_2_temp_m;  
  int32_t pt100_3_temp_m;  
  int32_t pt100_4_temp_m;    
  uint8_t leak_sensor;
  int16_t temperature;    
  uint16_t humidity;      
} SensCardStat_t;

extern SensCardStat_t SensCardStat;

typedef struct {
  uint16_t fan1_duty; // 0x0090
  uint16_t fan2_duty;  
  uint16_t fan3_duty;  
  uint16_t fan4_duty;  
  uint16_t fan5_duty;  
  uint16_t fan6_duty;  
  uint16_t fan7_duty;  
  uint16_t fan8_duty;  
  uint16_t fan9_duty;  
  uint16_t fan10_duty;  
  uint16_t fan11_duty;  
  uint16_t fan12_duty;  
  uint16_t fan13_duty;  
  uint16_t fan14_duty;  
  uint16_t fan15_duty;  
  uint16_t fan16_duty; // 0x009F
} FansCardCtrl_t;

extern FansCardCtrl_t FansCardCtrl;

typedef struct {
  uint16_t fan1_fb;  // 0x0080
  uint16_t fan2_fb;
  uint16_t fan3_fb;
  uint16_t fan4_fb;
  uint16_t fan5_fb;
  uint16_t fan6_fb;
  uint16_t fan7_fb;
  uint16_t fan8_fb;
  uint16_t fan9_fb;
  uint16_t fan10_fb;
  uint16_t fan11_fb;
  uint16_t fan12_fb;
  uint16_t fan13_fb;
  uint16_t fan14_fb;
  uint16_t fan15_fb;
  uint16_t fan16_fb;  // 0x008F
} FansCardStat_t;

extern FansCardStat_t FansCardStat;

void SideCardTaskFunc(void* pvParameters);
void ReadCardTaskFunc(void* pvParameters);

#endif  // SIDE_CARD_TASK_H