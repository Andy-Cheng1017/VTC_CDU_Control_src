#ifndef WARNING_TASK_H
#define WARNING_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t warning_handler;

void warning_task_function(void* pvParameters);

#endif  // WARNING_TASK_H