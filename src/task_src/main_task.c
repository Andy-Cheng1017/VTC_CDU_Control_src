#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main_task.h"
#include "pt100_task.h"
#include "alarm_task.h"
#include "pump_task.h"
#include "sensor_task.h"
#include "side_card_task.h"
#include "PID.h"

#define LOG_TAG "Main_Task"
#include "elog.h"

#define MAIN_PERIOD 20

TaskHandle_t main_handler;

SysInform_t SysInform = {
    .power_on_setting = 0,
};

SysParaSet_t SysParaSet = {
    .ctrl_mode = TEMP_CONST,
    .temp_set = 30000,
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

    if (SysInform.power_on_setting == 0) {
      SysParaSet.ctrl_mode = NON_CTRL;
    } else if (SysInform.power_on_setting == 1) {
      SysInform.power_on_setting = 3;
      SysParaSet.ctrl_mode = TEMP_CONST;
    } else if (SysInform.power_on_setting == 4) {
      SysInform.power_on_setting = 0;
      SysParaSet.ctrl_mode = TEMP_CONST;
    }

    if (SysParaSet.ctrl_mode == TEMP_CONST) {
      if (OUTLET_TEMP_CHANNEL > SysParaSet.temp_set) {
        pump_control.pump_1_rpm = 200;
        FanCardSysSet.auto_control_target_speed = 100;
      } else {
        FanCardSysSet.auto_control_target_speed = 0;
        pump_control.pump_1_rpm = 0;
      }
    } else if (SysParaSet.ctrl_mode == FLOW_CONST) {
    } else if (SysParaSet.ctrl_mode == PRESS_CONST) {
    } else if (SysParaSet.ctrl_mode == NON_CTRL) {
    }
  }
  vTaskDelete(NULL);
}