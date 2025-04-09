#ifndef ALARM_TASK_H
#define ALARM_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t alarm_handler;

void alarm_task_function(void* pvParameters);

#endif