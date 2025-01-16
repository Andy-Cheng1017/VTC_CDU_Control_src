#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LCD_task.h"

#define LOG_TAG "LCD_Task"
#include "elog.h"

TaskHandle_t LCD_handler;

rs485_func_t LCD_rx_Func = 0;
uint8_t LCD_rx_Data[LCD_DATA_MAX_SIZE] = {0};
uint8_t LCD_rx_Data_len = 0;

rs485_func_t LCD_tx_Func = 0;
uint8_t LCD_tx_Data[LCD_DATA_MAX_SIZE] = {0};
uint8_t LCD_tx_Data_len = 0;

bool Unpkg_Flag = FALSE;
bool Decode_Flag = FALSE;

rs485_t RS485_LCD = {
    .UART = UART8,
    .Mode = SLAVE,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .IpAddr = MY_485_ADDR,
};

void LCD_task_function(void* pvParameters) {
  RS485_init(&RS485_LCD);
  log_i("LCD Task Running");
  RS485_LCD.RegHdlerStat = 0x00;
  RS485_LCD.RegHdlerEnd = 0x2F;
  RS485_RegisterHandler(&RS485_LCD, SysStat_Handler);
  RS485_LCD.RegHdlerStat = 0x30;
  RS485_LCD.RegHdlerEnd = 0x5F;
  RS485_RegisterHandler(&RS485_LCD, DataRead_Handler);
  RS485_LCD.RegHdlerStat = 0x60;
  RS485_LCD.RegHdlerEnd = 0x7F;
  RS485_RegisterHandler(&RS485_LCD, DevCtrl_Handler);
  RS485_LCD.RegHdlerStat = 0x80;
  RS485_LCD.RegHdlerEnd = 0x8F;
  RS485_RegisterHandler(&RS485_LCD, EthConfig_Handler);

  while (1) {
    if (RS485_LCD.RxPkgCpltFlag == TRUE) {
      memset(LCD_rx_Data, 0, LCD_DATA_MAX_SIZE);
      rs485_error_t ret = RS485_Unpkg(&RS485_LCD, &LCD_rx_Func, LCD_rx_Data, &LCD_rx_Data_len);

      if (ret == UNPKG_FINISH) {
        log_i("485 Unpkg Finish");
      } else {
        log_i("LCD rx Func: %d", LCD_rx_Func);
        elog_hexdump("LCD_rx_Data", 16, LCD_rx_Data, sizeof(LCD_rx_Data));
        if (!ret) {
          log_i("LCD Unpkg Success");
          Unpkg_Flag = TRUE;
        } else if (ret == UNPKG_OVER_PACKGE_SIZE) {
          log_e("485 Over Package Size");
          Unpkg_Flag = FALSE;
        } else if (ret == CRC_ERROR) {
          log_e("485 CRC Error");
          Unpkg_Flag = FALSE;
        } else if (ret == OTHER_ADDR)
          log_i("485 Not My Address");
      }
    }

    if (LCD_rx_Func != 0 && Unpkg_Flag) {
      memset(LCD_tx_Data, 0, LCD_DATA_MAX_SIZE);
      rs485_error_t ret = RS485_Decode(&RS485_LCD, LCD_rx_Func, LCD_rx_Data, LCD_rx_Data_len, &LCD_tx_Func, LCD_tx_Data, &LCD_tx_Data_len);
      log_i("LCD tx Func: %d", LCD_tx_Func);
      elog_hexdump("LCD_tx_Data", 16, LCD_tx_Data, sizeof(LCD_tx_Data));
      if (!ret) {
        log_i("LCD Decode Success");
      } else if (ret == ILLIGAL_FUNC)
        log_e("485 Illegal Function");
      else if (ret == ILLIGAL_DATA_ADDR)
        log_e("485 Illegal Data Address");
      else if (ret == ILLIGAL_DATA_VALUE)
        log_e("485 Illegal Data Value");
      else if (ret == SLAVE_DEVICE_FAILURE)
        log_e("485 Slave Device Failure");
      LCD_rx_Func = 0;
      LCD_rx_Data_len = 0;
      memset(LCD_rx_Data, 0, LCD_DATA_MAX_SIZE);
      Unpkg_Flag = FALSE;
    }

    if (LCD_tx_Func != 0) {
      RS485_Pkg(&RS485_LCD, MY_485_ADDR, LCD_tx_Func, LCD_tx_Data, LCD_tx_Data_len);
      LCD_tx_Func = 0;
      LCD_tx_Data_len = 0;
      memset(LCD_tx_Data, 0, LCD_DATA_MAX_SIZE);
    }
    vTaskDelay(25);
  }
  vTaskDelete(LCD_handler);
}
