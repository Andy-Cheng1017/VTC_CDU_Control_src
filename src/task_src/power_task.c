#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "power_task.h"

#define LOG_TAG "Power_Task"
#include "elog.h"

TaskHandle_t power_handler;

PowerStat_t Power_1_Stat = {0};
PowerStat_t Power_2_Stat = {0};

void power_task_function(void* pvParameters) {
  log_i("Power Task Running");

  while (1) {
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}