#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LCD_task.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

#define LOG_TAG "LCD_Task"
#include "elog.h"

TaskHandle_t LCD_handler;

Rs485_t RsLCD = {
    .UART = UART8,
    .Mode = SLAVE,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .ip_addr = MY_485_ADDR,
    .root = false,
};

void UART8_IRQHandler(void) {
  if (usart_interrupt_flag_get(RsLCD.UART, USART_RDBF_FLAG) != RESET) {
    usart_flag_clear(RsLCD.UART, USART_RDBF_FLAG);
    RS485_Rx_Data_ISR(&RsLCD);

  } else if (usart_interrupt_flag_get(RsLCD.UART, USART_IDLEF_FLAG) != RESET) {
    usart_flag_clear(RsLCD.UART, USART_IDLEF_FLAG);
    RS485_Rx_Cplt_ISR(&RsLCD);

  } else if (usart_interrupt_flag_get(RsLCD.UART, USART_TDBE_FLAG) != RESET) {
    usart_flag_clear(RsLCD.UART, USART_TDBE_FLAG);
    usart_interrupt_enable(RsLCD.UART, USART_TDBE_INT, FALSE);
    RsLCD.tx_idex--;
    RS485_Tx_Data_ISR(&RsLCD);
  }
}

void LCD_task_function(void* pvParameters) {
  RsInit(&RsLCD);
  log_i("LCD Task Running");
  RsLCD.reg_hdle_stat = 0x00;
  RsLCD.reg_hdle_end = 0x2F;
  RsRegHdle(&RsLCD, SysStat_Handler);
  RsLCD.reg_hdle_stat = 0x30;
  RsLCD.reg_hdle_end = 0x5F;
  RsRegHdle(&RsLCD, DataRead_Handler);
  RsLCD.reg_hdle_stat = 0x60;
  RsLCD.reg_hdle_end = 0x7F;
  RsRegHdle(&RsLCD, DevCtrl_Handler);
  RsLCD.reg_hdle_stat = 0x80;
  RsLCD.reg_hdle_end = 0x9F;
  RsRegHdle(&RsLCD, FansCardHdle);

  RsError_t err;

  while (1) {
    if (RsChkAvailable(&RsLCD)) {
      err = RS485Read(&RsLCD);

      if (err == UNPKG_FINISH) {
        continue;
      } else if (err != RS485_OK) {
        log_e("Error reading from RS485: %d", err);
        continue;
      } else {
        log_i("RS485 Read Success");
        elog_hexdump("UPPER_rx_Data", 32, RsLCD.rx_Data, sizeof(RsLCD.rx_Data));
      }

      err = RS485ReadHandler(&RsLCD);

      if (err != RS485_OK) {
        log_e("Error handling RS485: %d", err);
        continue;
      } else {
        log_i("RS485 Handler Success");
      }

      err = RS485Write(&RsLCD);

      if (err != RS485_OK) {
        log_e("Error writing to RS485: %d", err);
        continue;
      } else {
        log_i("RS485 Write Success");
      }
    }
    vTaskDelay(25);
  }
  vTaskDelete(LCD_handler);
}
