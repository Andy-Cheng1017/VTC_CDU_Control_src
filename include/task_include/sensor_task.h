#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t sensor_handler;

typedef struct {
  int16_t PT100_1_temp;
  int16_t PT100_2_temp;
  int16_t PT100_3_temp;
  int16_t PT100_4_temp;
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
} sensor_status_type;

typedef struct {
  uint16_t Porpo_1_PWM;
  uint16_t Porpo_2_PWM;
} sensor_control_type;

void sensor_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H