#ifndef NETWORK_TASK_H
#define NETWORK_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t network_handler;

void network_task_function(void* pvParameters);

#endif  // NETWORK_TASK_H
