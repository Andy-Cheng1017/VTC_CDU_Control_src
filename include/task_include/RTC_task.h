#ifndef RTC_TASK_H
#define RTC_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t RTC_handler;

void RTC_task_function(void* pvParameters);

#endif  // RTC_TASK_H