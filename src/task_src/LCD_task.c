#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LCD_task.h"

#define LOG_TAG "LCD_Task"
#include "elog.h"

TaskHandle_t LCD_handler;

rs485_func_t LCD_rx_Func;
uint8_t LCD_rx_Data[LCD_DATA_MAX_SIZE];
uint8_t LCD_rx_Data_len;

rs485_func_t LCD_tx_Func;
uint8_t LCD_tx_Data[LCD_DATA_MAX_SIZE];
uint8_t LCD_tx_Data_len;

void LCD_task_function(void* pvParameters) {
  RS485_init(&RS485_LCD, UART8, BR_9600, USART_DATA_8BITS, USART_STOP_1_BIT, 0X21);
  RS485_RegisterHandler(0X0000, 0X00FF, SysStat_Handler);
  RS485_RegisterHandler(0X0030, 0X005F, DataRead_Handler);
  RS485_RegisterHandler(0X0060, 0X007F, DevCtrl_Handler);
  RS485_RegisterHandler(0X0080, 0X00FF, EthConfig_Handler);

  while (1) {
    if (RS485_LCD.RxPkgCpltFlag == TRUE) {
      rs485_error_t ret = RS485_Unpkg(&RS485_LCD, &LCD_rx_Func, LCD_rx_Data, &LCD_rx_Data_len);
      if (!ret) {
        log_i("LCD Unpkg Success");
        log_i("LCD Func: %d", LCD_rx_Func);
        log_i("LCD Data: %d", LCD_rx_Data);
      } else if (ret == UNPKG_OVER_PACKGE_SIZE)
        log_e("485 Over Package Size");
      else if (ret == CRC_ERROR)
        log_e("485 CRC Error");
      else if (ret == NOT_MY_ADDR)
        log_i("485 Not My Address");
    }

    if (LCD_rx_Func != 0) {
      memset(LCD_tx_Data, 0, LCD_DATA_MAX_SIZE);
      rs485_error_t ret = RS485_Decode(&RS485_LCD, LCD_rx_Func, LCD_rx_Data, LCD_rx_Data_len, &LCD_tx_Func, LCD_tx_Data, &LCD_tx_Data_len);
      if (!ret) {
        log_i("LCD Decode Success");
        log_i("LCD Func: %d", LCD_tx_Func);
        log_i("LCD Data: %d", LCD_tx_Data);
      } else if (ret == ILLIGAL_FUNC)
        log_e("485 Illegal Function");
      else if (ret == ILLIGAL_DATA_ADDR)
        log_e("485 Illegal Data Address");
      else if (ret == ILLIGAL_DATA_VALUE)
        log_e("485 Illegal Data Value");
      LCD_rx_Func = 0;
      LCD_rx_Data_len = 0;
      memset(LCD_rx_Data, 0, LCD_DATA_MAX_SIZE);
    }

    if (LCD_tx_Func != 0) {

    }
    vTaskDelay(25);
  }
  vTaskDelete(LCD_handler);
}
