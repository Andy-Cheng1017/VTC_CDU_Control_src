#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "pump_task.h"


#define LOG_TAG "Pump_Task"
#include "elog.h"

TaskHandle_t pump_handler;

void pump_task_function(void* pvParameters) {
    while (1) {
        vTaskDelay(500);
    }
}