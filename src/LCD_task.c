#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "LCD_task.h"


#define LOG_TAG "LCD_Task"
#include "elog.h"

TaskHandle_t LCD_handler = NULL;

void LCD_task_function(void* pvParameters) {
  while (1) {
    vTaskDelay(500);
  }
}
