#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "wk_usart.h"
#include "wk_crc.h"
#include "wk_dma.h"
#include "LCD_task.h"


#define LOG_TAG "LCD_Task"
#include "elog.h"

TaskHandle_t LCD_handler;

void LCD_task_function(void* pvParameters) {
  while (1) {
    vTaskDelay(500);
  }
  vTaskDelete(LCD_handler);
}
