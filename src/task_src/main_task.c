#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main_task.h"
#include "pump_task.h"
#include "sensor_task.h"
#include "pt100_task.h"
#include "side_card_task.h"
#include "threshold_monitor.h"

#define LOG_TAG "Main_Task"
#include "elog.h"

#define MAIN_PERIOD 20

#define SET_BIT_TO(var, bit, value) ((var) = (value) ? ((var) | (1 << (bit))) : ((var) & ~(1 << (bit))))

#define PRESS_OUTLET_LOW_WARN_BIT 0
#define PRESS_OUTLET_HIGH_WARN_BIT 1
#define PRESS_INLET_LOW_WARN_BIT 2
#define PRESS_INLET_HIGH_WARN_BIT 3
#define PRESS_RETURN_LOW_WARN_BIT 4
#define PRESS_RETURN_HIGH_WARN_BIT 5
#define FLOW_LOW_WARN_BIT 6
#define FLOW_HIGH_WARN_BIT 7
#define TEMP_OUTLET_LOW_WARN_BIT 8
#define TEMP_OUTLET_HIGH_WARN_BIT 9
#define TEMP_AMBIENT_HIGH_WARN_BIT 10
#define LEAK_CUD_WARN_BIT 11
#define LEAK_SERVER_WARN_BIT 12
#define LEAK_SIDECAR_WARN_BIT 13

#define INLET_PRESSURE_CHANNEL SensStat.press_1_val_kpa
#define OUTLET_PRESSURE_CHANNEL SensStat.press_2_val_kpa
#define RETURN_PRESSURE_CHANNEL SensCardStat.press_1_val_kpa

#define FLOW_CHANNEL SensStat.Flow_val

#define OUTLET_TEMP_CHANNEL Pt100Stat.pt100_1_temp_m
#define AMBIENT_TEMP_CHANNEL SensStat.temperature

#define LEAK_SENSOR_CHANNEL SensStat.leak_sensor
#define CUD_LEAK_CHANNEL SensCardStat.leak_sensor
#define SERVER_LEAK_CHANNEL SensCardStat.leak_sensor

#define PUMP_1_EXITS_BIT 0
#define PUMP_2_EXITS_BIT 1
#define TEMP_HUM_EXITS_BIT 2
#define PT100_1_EXITS_BIT 3
#define PT100_2_EXITS_BIT 4
#define PT100_3_EXITS_BIT 5
#define PT100_4_EXITS_BIT 6
#define NTC_1_EXITS_BIT 7
#define NTC_2_EXITS_BIT 8
#define NTC_3_EXITS_BIT 9
#define NTC_4_EXITS_BIT 10
#define PRESS_1_EXITS_BIT 11
#define PRESS_2_EXITS_BIT 12
#define PRESS_3_EXITS_BIT 13
#define PRESS_4_EXITS_BIT 14
#define FLOW_EXITS_BIT 15

ThresholdMonitor_t press_monitor = {0};
ThresholdMonitor_t flow_monitor = {0};
ThresholdMonitor_t temp_monitor = {0};
ThresholdMonitor_t leak_monitor = {0};

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


    if (check_threshold_with_delay(&press_monitor, OUTLET_PRESSURE_CHANNEL, SysParaSet.press_warn.outlet_low, SysParaSet.press_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_LOW_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_LOW_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&press_monitor, OUTLET_PRESSURE_CHANNEL, SysParaSet.press_warn.outlet_high, SysParaSet.press_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_HIGH_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_HIGH_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&press_monitor, INLET_PRESSURE_CHANNEL, SysParaSet.press_warn.inlet_low, SysParaSet.press_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_LOW_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_LOW_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&press_monitor, INLET_PRESSURE_CHANNEL, SysParaSet.press_warn.inlet_high, SysParaSet.press_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_HIGH_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_HIGH_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&press_monitor, RETURN_PRESSURE_CHANNEL, SysParaSet.press_warn.return_low, SysParaSet.press_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_LOW_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_LOW_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&press_monitor, RETURN_PRESSURE_CHANNEL, SysParaSet.press_warn.return_high, SysParaSet.press_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_HIGH_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_HIGH_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&flow_monitor, SensStat.Flow_val, SysParaSet.flow_warn.low_val, SysParaSet.flow_warn.low_delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, FLOW_LOW_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, FLOW_LOW_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&flow_monitor, SensStat.Flow_val, SysParaSet.flow_warn.high_val, SysParaSet.flow_warn.high_delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, FLOW_HIGH_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, FLOW_HIGH_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&temp_monitor, OUTLET_TEMP_CHANNEL, SysParaSet.temp_warn.outlet_low, SysParaSet.temp_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_LOW_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_LOW_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&temp_monitor, OUTLET_TEMP_CHANNEL, SysParaSet.temp_warn.outlet_high, SysParaSet.temp_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_HIGH_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_HIGH_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&temp_monitor, AMBIENT_TEMP_CHANNEL, SysParaSet.temp_warn.ambient_high, SysParaSet.temp_warn.delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_AMBIENT_HIGH_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_AMBIENT_HIGH_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&leak_monitor, LEAK_SENSOR_CHANNEL, 1, SysParaSet.leak_warn.CUD_delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, LEAK_CUD_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, LEAK_CUD_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&leak_monitor, SERVER_LEAK_CHANNEL, 1, SysParaSet.leak_warn.server_delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SERVER_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SERVER_WARN_BIT, false);
    }
    if (check_threshold_with_delay(&leak_monitor, CUD_LEAK_CHANNEL, 1, SysParaSet.leak_warn.sidecar_delay)) {
      SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SIDECAR_WARN_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SIDECAR_WARN_BIT, false);
    }

    if (SensStat.device_connected & (1 << PUMP_1_EXITS_BIT)) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PUMP_1_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PUMP_1_EXITS_BIT, false);
    }
    if (SensStat.device_connected & (1 << PUMP_2_EXITS_BIT)) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PUMP_2_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PUMP_2_EXITS_BIT, false);
    }
    if (SensStat.device_connected & (1 << TEMP_HUM_EXITS_BIT)) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, TEMP_HUM_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, TEMP_HUM_EXITS_BIT, false);
    }
    if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_1_temp_m || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_1_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_1_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PT100_1_EXITS_BIT, false);
    }
    if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_2_temp_m || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_2_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_2_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PT100_2_EXITS_BIT, false);
    }
    if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_3_temp_m || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_3_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_3_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PT100_3_EXITS_BIT, false);
    }
    if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_4_temp_m || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_4_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_4_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PT100_4_EXITS_BIT, false);
    }
    if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_1_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_1_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_1_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, NTC_1_EXITS_BIT, false);
    }
    if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_2_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_2_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_2_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, NTC_2_EXITS_BIT, false);
    }
    if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_3_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_3_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_3_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, NTC_3_EXITS_BIT, false);
    }
    if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_4_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_4_temp_m) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_4_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, NTC_4_EXITS_BIT, false);
    }
    if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_1_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_1_val_kpa) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_1_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_1_EXITS_BIT, false);
    }
    if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_2_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_2_val_kpa) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_2_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_2_EXITS_BIT, false);
    }
    if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_3_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_3_val_kpa) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_3_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_3_EXITS_BIT, false);
    }
    if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_4_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_4_val_kpa) {
      SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_4_EXITS_BIT, true);
    } else {
      SET_BIT_TO(SysParaDisp.warn_stat, PRESS_4_EXITS_BIT, false);
    }

    if (SysParaSet.ctrl_mode == TEMP_CONST) {
    } else if (SysParaSet.ctrl_mode == FLOW_CONST) {
    } else if (SysParaSet.ctrl_mode == PRESS_CONST) {
    } else if (SysParaSet.ctrl_mode == NON_CTRL) {
    }
  }
  vTaskDelete(NULL);
}