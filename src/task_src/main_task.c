#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main_task.h"
#include "pump_task.h"
#include "sensor_task.h"
#include "pt100_task.h"
#include "side_card_task.h"

#define LOG_TAG "Main_Task"
#include "elog.h"

#define MAIN_PERIOD 500

TaskHandle_t main_handler;

void main_task_function(void* pvParameters) {
  log_i("Main Task Started");
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, MAIN_PERIOD);
    if (SensCardStat.pt100_2_temp_m > 22000 && SensCardStat.pt100_2_temp_m < 70000) {
      pump_control.pump_1_rpm = 200;
      FansCardCtrl.fan_duty[0] = 200;
      FansCardCtrl.fan_duty[1] = 200;
      FansCardCtrl.fan_duty[2] = 200;
      FansCardCtrl.fan_duty[3] = 200;

    } else {
      pump_control.pump_1_rpm = 100;
      FansCardCtrl.fan_duty[0] = 0;
      FansCardCtrl.fan_duty[1] = 0;
      FansCardCtrl.fan_duty[2] = 0;
      FansCardCtrl.fan_duty[3] = 0;
    }
  }
}