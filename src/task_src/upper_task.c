#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "upper_task.h"
#include "main.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

#define LOG_TAG "UPPER_Task"
#include "elog.h"

TaskHandle_t upper_handler;

Rs485_t RsUpper = {
    .UART = UART4,
    .Mode = SLAVE,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .ip_addr = MY_485_ADDR,
    .root = false,
};

void UART4_IRQHandler(void) {
  if (usart_interrupt_flag_get(RsUpper.UART, USART_RDBF_FLAG) != RESET) {
    usart_flag_clear(RsUpper.UART, USART_RDBF_FLAG);
    RS485_Rx_Data_ISR(&RsUpper);

  } else if (usart_interrupt_flag_get(RsUpper.UART, USART_IDLEF_FLAG) != RESET) {
    usart_flag_clear(RsUpper.UART, USART_IDLEF_FLAG);
    RS485_Rx_Cplt_ISR(&RsUpper);

  } else if (usart_interrupt_flag_get(RsUpper.UART, USART_TDBE_FLAG) != RESET) {
    usart_flag_clear(RsUpper.UART, USART_TDBE_FLAG);
    usart_interrupt_enable(RsUpper.UART, USART_TDBE_INT, FALSE);
    RS485_Tx_Data_ISR(&RsUpper);
  }
}

void upper_task_function(void* pvParameters) {
  RsInit(&RsUpper);
  log_i("UPPER Task Running");

  RsError_t err;

  while (1) {
    if (RsChkAvailable(&RsUpper)) {
      err = RS485Read(&RsUpper);

      if (err == UNPKG_FINISH) {
        continue;
      } else if (err != RS485_OK) {
        log_e("Error reading from RS485: %d", err);
        continue;
      } else {
        // log_i("RS485 Read Success");
        // elog_hexdump("UPPER_rx_Data", 16, RsUpper.rx_Data, sizeof(RsUpper.rx_Data)/2);
      }

      xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);
      err = RS485ReadHandler(&RsUpper);
      xSemaphoreGive(RS485RegionMutex);

      if (err != RS485_OK) {
        log_e("Error handling RS485: %d", err);
        continue;
      } else {
        // log_i("RS485 Handler Success");
      }

      err = RS485Write(&RsUpper);

      if (err != RS485_OK) {
        log_e("Error writing to RS485: %d", err);
        continue;
      } else {
        // log_i("RS485 Write Success");
      }
    }

    vTaskDelay(25);
  }
  vTaskDelete(upper_handler);
}