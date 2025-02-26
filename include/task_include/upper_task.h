#ifndef UPPER_TASK_H
#define UPPER_TASK_H

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

extern TaskHandle_t upper_handler;

extern Rs485_t RsUpper;

void upper_task_function(void* pvParameters);

#endif  // UPPER_TASK_H