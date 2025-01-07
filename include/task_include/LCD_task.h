#ifndef LCD_TASK_H
#define LCD_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "RS485.h"

extern TaskHandle_t LCD_handler;

#define LCD_DATA_MAX_SIZE 64

rs485_t RS485_LCD;

void LCD_task_function(void* pvParameters);

#endif  // LCD_TASK_H