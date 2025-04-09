#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main_task.h"
#include "PID.h"

#define LOG_TAG "Main_Task"
#include "elog.h"

#define MAIN_PERIOD 20

TaskHandle_t main_handler;

SysParaSet_t SysParaSet = {
    .ctrl_mode = TEMP_CONST,
    .temp_set = 30,
    .flow_set = 0,
    .press_set = 0,
    .pump_min_duty = 100,
    .pump_stop_delay = 100,

    .press_warn =
        {
            .act = ONLY_WARNING,
            .delay = 30,
            .inlet_low = 200,
            .inlet_high = 700,
            .return_low = 200,
            .return_high = 700,
            .outlet_low = 200,
            .outlet_high = 700,
        },
    .flow_warn =
        {
            .act = ONLY_WARNING,
            .low_val = 0,
            .low_delay = 100,
            .high_val = 0,
            .high_delay = 100,
        },
    .temp_warn =
        {
            .act = ONLY_WARNING,
            .delay = 30,
            .outlet_low = 20,
            .outlet_high = 80,
            .ambient_high = 35,
        },
    .leak_warn =
        {
            .act = ONLY_WARNING,
            .CUD_delay = 30,
            .server_delay = 30,
            .sidecar_delay = 30,
        },

    .pt100_abnl_temp_low_m = 5000,
    .pt100_abnl_temp_high_m = 105000,
    .ntc_abnl_temp_low_m = 5000,
    .ntc_abnl_temp_high_m = 105000,
    .press_abnl_val_low_kpa = 0,
    .press_abnl_val_high_kpa = 10000,
};

SysParaDisp_t SysParaDisp = {0};

void main_task_function(void* pvParameters) {
  log_i("Main Task Started");

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, MAIN_PERIOD);

    if (SysParaSet.ctrl_mode == TEMP_CONST) {
    } else if (SysParaSet.ctrl_mode == FLOW_CONST) {
    } else if (SysParaSet.ctrl_mode == PRESS_CONST) {
    } else if (SysParaSet.ctrl_mode == NON_CTRL) {
    }
  }
  vTaskDelete(NULL);
}