#ifndef FANS_TASK_H
#define FANS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t fans_handler;

void fans_task_function(void* pvParameters);

#endif  // FANS_TASK_H