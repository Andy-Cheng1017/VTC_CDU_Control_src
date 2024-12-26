#ifndef LCD_TASK_H
#define LCD_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t LCD_handler;

void LCD_task_function(void* pvParameters);

#endif  // LCD_TASK_H