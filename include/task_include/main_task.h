#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t main_handler;

void main_task_function(void* pvParameters);

#endif