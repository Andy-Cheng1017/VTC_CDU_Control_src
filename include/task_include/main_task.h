#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "PID.h"

extern TaskHandle_t main_handler;

typedef enum {
  NON_CTRL,
  TEMP_CONST,
  FLOW_CONST,
  PRESS_CONST,
} CtrlMode_t;

typedef enum {
  ONLY_WARNING,
  WARNING_AND_STOP,
} AlarmAction_t;

typedef struct {
  AlarmAction_t act;
  int16_t delay;
  uint16_t outlet_low;
  uint16_t outlet_high;
  uint16_t inlet_low;
  uint16_t inlet_high;
  uint16_t return_low;
  uint16_t return_high;
} PressureWarn_t;

typedef struct {
  AlarmAction_t act;
  uint16_t low_val;
  uint16_t low_delay;
  uint16_t high_val;
  uint16_t high_delay;
} FlowWarn_t;

typedef struct {
  AlarmAction_t act;
  uint16_t delay;
  int16_t outlet_low;
  int16_t outlet_high;
  uint16_t ambient_high;
} TempWarn_t;

typedef struct {
  AlarmAction_t act;
  uint16_t CUD_delay;
  uint16_t server_delay;
  uint16_t sidecar_delay;
} LeakWarn_t;

typedef struct {
  uint16_t power_on_setting;
} SysInform_t;

extern SysInform_t SysInform;

typedef struct {
  CtrlMode_t ctrl_mode;
  int32_t temp_set;
  int16_t flow_set;
  int16_t press_set;
  uint16_t pump_min_duty;
  uint16_t pump_stop_delay;

  PressureWarn_t press_warn;

  FlowWarn_t flow_warn;

  TempWarn_t temp_warn;

  LeakWarn_t leak_warn;

  int32_t dew_temp_calib;
  int32_t ambient_temp_calib;
  int32_t inlet_temp_calib;
  int32_t outlet_temp_calib;

  int32_t pt100_abnl_temp_low_m;
  int32_t pt100_abnl_temp_high_m;
  int32_t ntc_abnl_temp_low_m;
  int32_t ntc_abnl_temp_high_m;
  int16_t press_abnl_val_low_kpa;
  int16_t press_abnl_val_high_kpa;

} SysParaSet_t;

extern SysParaSet_t SysParaSet;

typedef struct {
  uint16_t warn_stat;

  uint16_t sys_startup_time_high;
  uint16_t sys_startup_time_low;
  uint16_t pump_1_startup_time_high;
  uint16_t pump_1_startup_time_low;
  uint16_t pump_2_startup_time_high;
  uint16_t pump_2_startup_time_low;

  uint16_t periphery_component_exist;
} SysParaDisp_t;

extern SysParaDisp_t SysParaDisp;

extern PIDController CUD_PID;

void main_task_function(void* pvParameters);

#endif