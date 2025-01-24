#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Two_Pt_Cal.h"

extern TaskHandle_t sensor_handler;

typedef struct {
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
} SensCtrl_t;

extern SensCtrl_t SensCtrl;

void sensor_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H