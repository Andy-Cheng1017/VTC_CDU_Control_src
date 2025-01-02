#ifndef PUMP_TASK_H
#define PUMP_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t pump_handler;

typedef struct {
  uint16_t pump_1_rpm;
  uint16_t pump_2_rpm;
} pump_control_type;

typedef struct {
  uint16_t pump_1_FB;
  uint16_t pump_2_FB;
} pump_status_type;

void pump_task_function(void* pvParameters);

#endif  // PUMP_TASK_H