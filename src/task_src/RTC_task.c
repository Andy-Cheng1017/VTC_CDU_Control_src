#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "RTC_task.h"


#define LOG_TAG "RTC_Task"
#include "elog.h"

TaskHandle_t RTC_handler;

void RTC_task_function(void* pvParameters) {
    while (1) {
        vTaskDelay(500);
    }
}