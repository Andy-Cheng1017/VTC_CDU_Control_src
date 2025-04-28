#include "FreeRTOS.h"
#include "task.h"
#include "alarm_task.h"
#include "pump_task.h"
#include "sensor_task.h"
#include "pt100_task.h"
#include "side_card_task.h"
#include "threshold_monitor.h"
#include "main_task.h"

#define LOG_TAG "Alarm_Task"
#include "elog.h"

TaskHandle_t alarm_handler;

#define ALARM_PERIOD 20

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

void alarm_task_function(void* pvParameters) {
  //   log_i("Alarm Task Started");
  //   TickType_t xLastWakeTime = xTaskGetTickCount();
  //   while (1) {
  //     if (over_threshold_with_delay(&press_monitor, OUTLET_PRESSURE_CHANNEL, SysParaSet.press_warn.outlet_low, SysParaSet.press_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_LOW_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_LOW_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&press_monitor, OUTLET_PRESSURE_CHANNEL, SysParaSet.press_warn.outlet_high, SysParaSet.press_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_HIGH_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_OUTLET_HIGH_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&press_monitor, INLET_PRESSURE_CHANNEL, SysParaSet.press_warn.inlet_low, SysParaSet.press_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_LOW_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_LOW_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&press_monitor, INLET_PRESSURE_CHANNEL, SysParaSet.press_warn.inlet_high, SysParaSet.press_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_HIGH_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_INLET_HIGH_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&press_monitor, RETURN_PRESSURE_CHANNEL, SysParaSet.press_warn.return_low, SysParaSet.press_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_LOW_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_LOW_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&press_monitor, RETURN_PRESSURE_CHANNEL, SysParaSet.press_warn.return_high, SysParaSet.press_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_HIGH_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_RETURN_HIGH_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&flow_monitor, SensStat.Flow_val, SysParaSet.flow_warn.low_val, SysParaSet.flow_warn.low_delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, FLOW_LOW_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, FLOW_LOW_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&flow_monitor, SensStat.Flow_val, SysParaSet.flow_warn.high_val, SysParaSet.flow_warn.high_delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, FLOW_HIGH_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, FLOW_HIGH_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&temp_monitor, OUTLET_TEMP_CHANNEL, SysParaSet.temp_warn.outlet_low, SysParaSet.temp_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_LOW_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_LOW_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&temp_monitor, OUTLET_TEMP_CHANNEL, SysParaSet.temp_warn.outlet_high, SysParaSet.temp_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_HIGH_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_OUTLET_HIGH_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&temp_monitor, AMBIENT_TEMP_CHANNEL, SysParaSet.temp_warn.ambient_high, SysParaSet.temp_warn.delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_AMBIENT_HIGH_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_AMBIENT_HIGH_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&leak_monitor, LEAK_SENSOR_CHANNEL, 1, SysParaSet.leak_warn.CUD_delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, LEAK_CUD_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, LEAK_CUD_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&leak_monitor, SERVER_LEAK_CHANNEL, 1, SysParaSet.leak_warn.server_delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SERVER_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SERVER_WARN_BIT, false);
  //     }
  //     if (over_threshold_with_delay(&leak_monitor, CUD_LEAK_CHANNEL, 1, SysParaSet.leak_warn.sidecar_delay)) {
  //       SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SIDECAR_WARN_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, LEAK_SIDECAR_WARN_BIT, false);
  //     }

  //     if (SensStat.device_connected & (1 << PUMP_1_EXITS_BIT)) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PUMP_1_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PUMP_1_EXITS_BIT, false);
  //     }
  //     if (SensStat.device_connected & (1 << PUMP_2_EXITS_BIT)) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PUMP_2_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PUMP_2_EXITS_BIT, false);
  //     }
  //     if (SensStat.device_connected & (1 << TEMP_HUM_EXITS_BIT)) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, TEMP_HUM_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, TEMP_HUM_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_temp_x10[0] || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_temp_x10[0]) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_1_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PT100_1_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_temp_x10[1] || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_temp_x10[1]) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_2_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PT100_2_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_temp_x10[2] || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_temp_x10[2]) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_3_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PT100_3_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.pt100_abnl_temp_high_m < Pt100Stat.pt100_temp_x10[3] || SysParaSet.pt100_abnl_temp_low_m > Pt100Stat.pt100_temp_x10[3]) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PT100_4_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PT100_4_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_1_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_1_temp_m) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_1_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, NTC_1_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_2_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_2_temp_m) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_2_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, NTC_2_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_3_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_3_temp_m) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_3_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, NTC_3_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.ntc_abnl_temp_high_m < SensStat.ntc_4_temp_m || SysParaSet.ntc_abnl_temp_low_m > SensStat.ntc_4_temp_m) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, NTC_4_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, NTC_4_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_1_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_1_val_kpa) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_1_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_1_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_2_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_2_val_kpa) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_2_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_2_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_3_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_3_val_kpa) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_3_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_3_EXITS_BIT, false);
  //     }
  //     if (SysParaSet.press_abnl_val_high_kpa < SensStat.press_4_val_kpa || SysParaSet.press_abnl_val_low_kpa > SensStat.press_4_val_kpa) {
  //       SET_BIT_TO(SysParaDisp.periphery_component_exist, PRESS_4_EXITS_BIT, true);
  //     } else {
  //       SET_BIT_TO(SysParaDisp.warn_stat, PRESS_4_EXITS_BIT, false);
  //     }

  //     vTaskDelayUntil(&xLastWakeTime, ALARM_PERIOD);
  //   }
}
