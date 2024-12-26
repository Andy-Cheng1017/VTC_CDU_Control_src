#ifndef PUMP_TASK_H
#define PUMP_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t pump_handler;

void pump_task_function(void* pvParameters);

#endif  // PUMP_TASK_H