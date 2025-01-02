#include <stdio.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "RTC_task.h"
#include "RX8025T_wrap.h"


#define LOG_TAG "RTC_Task"
#include "elog.h"

TaskHandle_t RTC_handler;

void RTC_task_function(void* pvParameters) {
    RX8025T_init();
    time_t t = RX8025T_get();
    log_i("Current time is: %s", ctime(&t));
    while (1) {
        vTaskDelay(500);
    }
}