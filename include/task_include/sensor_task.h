#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "wk_adc.h"
#include "wk_dma.h"
#include "NTC.h"
#include "SensConvVal.h"
#include "Two_Pt_Cal.h"

extern TaskHandle_t sensor_handler;

typedef struct {
  int32_t ntc_1_temp;
  int32_t ntc_2_temp;
  int32_t ntc_3_temp;
  int32_t ntc_4_temp;
  int16_t press_1_val;
  int16_t press_2_val;
  int16_t press_3_val;
  int16_t press_4_val;
  int16_t Flow_val;
  uint16_t voltage_input;
  uint16_t current_input;
  int32_t power_input;
} SensStat_t;

extern SensStat_t SensStat;

typedef struct {
  uint16_t porpo_1_pwm;
  uint16_t porpo_2_pwm;
} SensCtrl_t;

extern SensCtrl_t SensCtrl;

typedef struct {
  int32_t ntc_1_raw_l_val;
  int32_t ntc_2_raw_l_val;
  int32_t ntc_3_raw_l_val;
  int32_t ntc_4_raw_l_val;
  int32_t ntc_1_raw_h_val;
  int32_t ntc_2_raw_h_val;
  int32_t ntc_3_raw_h_val;
  int32_t ntc_4_raw_h_val;
  int32_t ntc_ideal_l_val;
  int32_t ntc_ideal_h_val;
} NtcTwoCal_t;

extern NtcTwoCal_t NtcTwoCal;

typedef struct {
  int16_t press_1_raw_l_val;
  int16_t press_2_raw_l_val;
  int16_t press_3_raw_l_val;
  int16_t press_4_raw_l_val;
  int16_t press_1_raw_h_val;
  int16_t press_2_raw_h_val;
  int16_t press_3_raw_h_val;
  int16_t press_4_raw_h_val;
  int16_t press_ideal_l_val;
  int16_t press_ideal_h_val;
} PressTwoCal_t;

extern PressTwoCal_t PressTwoCal;

void sensor_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H