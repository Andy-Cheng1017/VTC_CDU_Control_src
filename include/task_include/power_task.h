#ifndef POWER_TASK_H
#define POWER_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t power_handler;

void power_task_function(void* pvParameters);

typedef struct {
  uint16_t power_status;
  uint16_t voltage_input;
  uint16_t current_input;
  int16_t power_temp;
} PowerStat_t;

extern PowerStat_t Power_1_Stat;
extern PowerStat_t Power_2_Stat;
#endif