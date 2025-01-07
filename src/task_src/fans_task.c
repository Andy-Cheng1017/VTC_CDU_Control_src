#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "fans_task.h"


#define LOG_TAG "Fans_Task"
#include "elog.h"

TaskHandle_t fans_handler;

fans_control_type fans_control = {0};
fans_status_type fans_status = {0};

void fans_task_function(void* pvParameters) {
    while (1) {
        vTaskDelay(500);
    }
}