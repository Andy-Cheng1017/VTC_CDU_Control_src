#ifndef LCD_TASK_H
#define LCD_TASK_H

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#define LCD_DATA_MAX_SIZE 32

extern TaskHandle_t LCD_handler;

void LCD_task_function(void* pvParameters);

#endif  // LCD_TASK_H