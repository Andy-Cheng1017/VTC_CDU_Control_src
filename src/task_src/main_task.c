#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main_task.h"
#include "pt100_task.h"
#include "alarm_task.h"
#include "pump_task.h"
#include "sensor_task.h"
#include "RS485_Region_handler_enum.h"
#include "side_card_task.h"
#include "RS485.h"

#define LOG_TAG "Main_Task"
#include "elog.h"

#define MAIN_PERIOD 100

#define INITIAL_KP 20.0f
#define INITIAL_KI 0.0f
#define INITIAL_KD 0.0f

PIDController CUD_PID = {
    .kp = INITIAL_KP,
    .ki = INITIAL_KI,
    .kd = INITIAL_KD,
    .G_base = 1.0f,
    .gain_update_interval = 1.0f,
    .dt = 0.1f,
    .alpha = 0.2f,
    .output_min = 0,
    .output_max = 1000,
};

TaskHandle_t main_handler;

SysInform_t SysInform = {
    .power_on_setting = 0,
};

SysParaSet_t SysParaSet = {
    .ctrl_mode = TEMP_CONST,
    .temp_set = 40000,
    .flow_set = 0,
    .press_set = 0,
    .pump_min_duty = 300,
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

    .pt100_abnl_temp_low_m = 50,
    .pt100_abnl_temp_high_m = 1050,
    .ntc_abnl_temp_low_m = 50,
    .ntc_abnl_temp_high_m = 1050,
    .press_abnl_val_low_kpa = 0,
    .press_abnl_val_high_kpa = 10000,
};

SysParaDisp_t SysParaDisp = {0};

uint16_t triger_fan = 0;

void main_task_function(void* pvParameters) {
  log_i("Main Task Started");

  TickType_t xLastWakeTime = xTaskGetTickCount();
  pump_control.pump_1_rpm = 300;

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, MAIN_PERIOD);

    if (SysInform.power_on_setting == 0) {
      SysParaSet.ctrl_mode = NON_CTRL;
    } else if (SysInform.power_on_setting == 1) {
      SysInform.power_on_setting = 3;
      SysParaSet.ctrl_mode = TEMP_CONST;
    } else if (SysInform.power_on_setting == 4) {
      SysInform.power_on_setting = 0;
      SysParaSet.ctrl_mode = NON_CTRL;
      FanCardSysSet.auto_control_target_speed = 0;
    }

    if (SysParaSet.ctrl_mode == TEMP_CONST) {
      FanCardSysSet.auto_control_target_speed = PID_Update(&CUD_PID, ((float)SysParaSet.temp_set) / 10.0f, ((float)OUTLET_TEMP_CHANNEL) / 1000.0f);

    } else if (SysParaSet.ctrl_mode == FLOW_CONST) {
    } else if (SysParaSet.ctrl_mode == PRESS_CONST) {
    } else if (SysParaSet.ctrl_mode == NON_CTRL) {
    }
    if (triger_fan != FanCardSysSet.auto_control_target_speed) {
      triger_fan = FanCardSysSet.auto_control_target_speed;
      log_i("Fan Speed Set: %d", FanCardSysSet.auto_control_target_speed);
      WRITE_CARD_SINGLE(FAN_RS485_ADDR, AUTO_CONTROL_TARGET_SPEED, FanCardSysSet.auto_control_target_speed);
    }
  }
  vTaskDelete(NULL);
}