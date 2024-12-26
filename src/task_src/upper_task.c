#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "upper_task.h"


#define LOG_TAG "Upper_Task"
#include "elog.h"

TaskHandle_t upper_handler = NULL;

void upper_task_function(void* pvParameters) {
    while (1) {
        vTaskDelay(500);
    }
}