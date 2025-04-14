#ifndef SIDE_CARD_TASK_H
#define SIDE_CARD_TASK_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t SideCardHandler;
extern TaskHandle_t ReadCardHandler;

typedef struct {
  int32_t pt100_1_temp_m;
  int32_t pt100_2_temp_m;
  int32_t pt100_3_temp_m;
  int32_t pt100_4_temp_m;
  int16_t press_1_val_kpa;
  int16_t press_2_val_kpa;
  uint8_t leak_sensor;
  int16_t temperature;
  uint16_t humidity;
} SensCardStat_t;

extern SensCardStat_t SensCardStat;

typedef struct {
  uint16_t pressure_pump;
} SensCardCtrl_t;

extern SensCardCtrl_t SensCardCtrl;

typedef struct {
  uint16_t fan_pwm[16];
} FansCardCtrl_t;

extern FansCardCtrl_t FansCardCtrl;

typedef struct {
  uint16_t fan_fg[16];  // 0x0080
} FansCardStat_t;

extern FansCardStat_t FansCardStat;

typedef enum {
  ONLY_WARNING_FAN,
  WARNING_AND_STOP_FAN,
} AlarmAction_FAN_t;

typedef struct {
  AlarmAction_FAN_t act;
  int16_t delay;
  uint16_t fan_low_speed_warning_threshold;
  uint16_t fan_fg_difference_warning_threshold;
} FanCardAlarm_t;

typedef struct {
  bool auto_control;
  uint16_t auto_control_target_speed;
  uint16_t fan_installation_status;

  FanCardAlarm_t fan_alarm;

} FanCardSysSet_t;

extern FanCardSysSet_t FanCardSysSet;

typedef struct {
  uint16_t fan_board_fault_status;
  uint16_t fan_status_on_fan_board_bitfield_0_15;

} FanCardSysDisp_t;

extern FanCardSysDisp_t FanCardSysDisp;

void SideCardTaskFunc(void* pvParameters);
void ReadCardTaskFunc(void* pvParameters);

#endif  // SIDE_CARD_TASK_H