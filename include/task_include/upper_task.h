#ifndef UPPER_TASK_H
#define UPPER_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t upper_handler;

void upper_task_function(void* pvParameters);

#endif  // UPPER_TASK_H