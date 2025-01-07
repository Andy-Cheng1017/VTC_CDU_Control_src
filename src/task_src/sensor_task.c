#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "sensor_task.h"


#define LOG_TAG "Sensor_Task"
#include "elog.h"

TaskHandle_t sensor_handler;

sensor_status_type sensor_status = {0};
sensor_control_type sensor_control = {0};

void sensor_task_function(void* pvParameters) {
    while (1) {
        // Add sensor reading and processing code here
        vTaskDelay(500);
    }
}