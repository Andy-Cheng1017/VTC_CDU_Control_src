#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LCD_task.h"

#define LOG_TAG "LCD_Task"
#include "elog.h"

TaskHandle_t LCD_handler;

rs485_func_t LCD_rx_Func;
uint8_t LCD_rx_Data_len;
uint8_t LCD_rx_Data[LCD_DATA_MAX_SIZE];

rs485_func_t LCD_tx_Func;
uint8_t LCD_tx_Data[LCD_DATA_MAX_SIZE];

void LCD_task_function(void* pvParameters) {
  RS485_init(RS485_LCD, UART8, BR_9600, USART_DATA_8BITS, USART_STOP_1_BIT, 0X21);

  while (1) {
    if (RS485_LCD.RxPkgCpltFlag == TRUE) {
      uint8_t ret = RS485_Unpkg(RS485_LCD, LCD_rx_Func, LCD_rx_Data, LCD_rx_Data_len);
      if (!ret) {
        log_i("LCD Unpkg Success");
        log_i("LCD Func: %d", LCD_rx_Func);
        log_i("LCD Data: %d", LCD_rx_Data);
        RS485_Decode(RS485_LCD, LCD_rx_Func, LCD_rx_Data, LCD_rx_Data_len);
      } else if (ret == UNPKG_OVER_PACKGE_SIZE)
        log_e("485 Over Package Size");
      else if (ret == CRC_ERROR)
        log_e("485 CRC Error");
      else if (ret == NOT_MY_ADDR)
        log_i("485 Not My Address");
    }

    vTaskDelay(25);
  }
  vTaskDelete(LCD_handler);
}
