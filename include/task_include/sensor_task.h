#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pt100.h"
#include "MCP342x_wrap.h"
#include "Two_Pt_Cal.h"

extern TaskHandle_t sensor_handler;

typedef struct {
  int32_t pt100_1_temp_m;
  int32_t pt100_2_temp_m;
  int32_t pt100_3_temp_m;
  int32_t pt100_4_temp_m;
  int16_t NTC_1_temp;
  int16_t NTC_2_temp;
  int16_t NTC_3_temp;
  int16_t NTC_4_temp;
  int16_t Presure_1_val;
  int16_t Presure_2_val;
  int16_t Presure_3_val;
  int16_t Presure_4_val;
  int16_t Flow_val;
  uint16_t voltage_input;
  uint16_t current_input;
  uint32_t power_input;
} SensStat_t;

extern SensStat_t SensStat;

typedef struct {
  uint16_t porpo_1_pwm;
  uint16_t porpo_2_pwm;
  uint32_t pt100_1_raw_l_val;
  uint32_t pt100_2_raw_l_val;
  uint32_t pt100_3_raw_l_val;
  uint32_t pt100_4_raw_l_val;
  uint32_t pt100_1_raw_h_val;
  uint32_t pt100_2_raw_h_val;
  uint32_t pt100_3_raw_h_val;
  uint32_t pt100_4_raw_h_val;
  uint32_t pt100_ideal_l_val;
  uint32_t pt100_ideal_h_val;
} SensCtrl_t;

extern SensCtrl_t SensCtrl;

MCP342x_error_t err;

void sensor_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H