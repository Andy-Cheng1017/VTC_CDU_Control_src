#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "warning_task.h"


#define LOG_TAG "Warning_Task"
#include "elog.h"

TaskHandle_t warning_handler;

void warning_task_function(void* pvParameters) {
    while (1) {
        vTaskDelay(500);
    }
}