#ifndef TEMP_HUM_TASK_H
#define TEMP_HUM_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t temp_hum_handler;

void temp_hum_task_function(void *pvParameters);

#endif // TEMP_HUM_TASK_H