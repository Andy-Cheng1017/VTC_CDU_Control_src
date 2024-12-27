#ifndef INIT_TASK_H
#define INIT_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t init_handler;

void init_task_function(void* pvParameters);

#endif  // INIT_TASK_H