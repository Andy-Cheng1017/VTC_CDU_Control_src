#include "LCD_task.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "RS485.h"
#include "RS485_Region_handler.h"
#include "main.h"
#include "side_card_task.h"

#define LOG_TAG "LCD_Task"
#include "elog.h"

#define SINGLE_DATA_MAX_SIZE 512

TaskHandle_t LCD_handler;

DECLARE_RS485_BUFFERS(RsLCD, SINGLE_DATA_MAX_SIZE);

Rs485_t RsLCD = {
    .UART = UART8,
    .Mode = SLAVE,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .ip_addr = CDU_RS485_ADDR,
    .root = false,

    RS485_BUFFERS_INIT(RsLCD, SINGLE_DATA_MAX_SIZE),

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
    RS485_Tx_Data_ISR(&RsLCD);
  }
}

void LCD_task_function(void* pvParameters) {
  log_i("LCD Task Running");

  RsInit(&RsLCD);

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
        // log_i("RS485 Read Success");
        // elog_hexdump("UPPER_rx_Data", 32, RsLCD.rx_Data, sizeof(RsLCD.rx_Data)/2);
      }

      xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);
      err = RS485ReadHandler(&RsLCD);
      xSemaphoreGive(RS485RegionMutex);

      if (IS_SENSCARD_WRITABLE_REG(RsLCD.rx_Func, RsLCD.rx_reg_start_addr)) {
        if (RsLCD.rx_Func == WRITE_SINGLE_REGISTER) {
          WRITE_CARD_SINGLE(SENS_RS485_ADDR, RsLCD.rx_reg_start_addr, RsLCD.rx_Data[2] << 8 | RsLCD.rx_Data[3]);
        }
      } else if (IS_FANCARD_WRITABLE_REG(RsLCD.rx_Func, RsLCD.rx_reg_start_addr)) {
        if (RsLCD.rx_Func == WRITE_SINGLE_REGISTER) {
          WRITE_CARD_SINGLE(FAN_RS485_ADDR, RsLCD.rx_reg_start_addr, RsLCD.rx_Data[2] << 8 | RsLCD.rx_Data[3]);
        }
      }

      if (err != RS485_OK) {
        log_e("Error handling RS485: %d", err);
        continue;
      } else {
        // log_i("RS485 Handler Success");
      }

      err = RS485Write(&RsLCD);

      if (err != RS485_OK) {
        log_e("Error writing to RS485: %d", err);
        continue;
      } else {
        // log_i("RS485 Write Success");
      }
    }
    vTaskDelay(25);
  }
  vTaskDelete(LCD_handler);
}
