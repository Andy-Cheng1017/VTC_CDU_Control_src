#ifndef UPPER_TASK_H
#define UPPER_TASK_H

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

#define UPPER_DATA_MAX_SIZE 32

extern TaskHandle_t upper_handler;

extern Rs485_t RS485_UPPER;

void upper_task_function(void* pvParameters);

#endif  // UPPER_TASK_H