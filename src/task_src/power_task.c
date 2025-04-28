#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "power_task.h"

#define LOG_TAG "Power_Task"
#include "elog.h"

TaskHandle_t power_handler;

PowerStat_t Power_Stat[2] = {
    [0] =
        {
            .power_status = 0,
            .voltage_input = 0,
            .current_input = 0,
            .power_temp = 0,
        },
    [1] =
        {
            .power_status = 0,
            .voltage_input = 0,
            .current_input = 0,
            .power_temp = 0,
        },
};

void power_task_function(void* pvParameters) {
  log_i("Power Task Running");

  while (1) {
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}