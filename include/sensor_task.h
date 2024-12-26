#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t sensor_handler;

void sensor_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H