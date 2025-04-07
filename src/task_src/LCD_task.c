#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LCD_task.h"
#include "main.h"
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
    RS485_Tx_Data_ISR(&RsLCD);
  }
}

void LCD_task_function(void* pvParameters) {
  RsInit(&RsLCD);
  log_i("LCD Task Running");
  RsLCD.reg_hdle_stat = SYSINFOM_REG_START;
  RsLCD.reg_hdle_end = SYSINFOM_REG_END;
  RsRegHdle(&RsLCD, SysInfom_Handler);
  RsLCD.reg_hdle_stat = SYSPARASET_REG_START;
  RsLCD.reg_hdle_end = SYSPARASET_REG_END;
  RsRegHdle(&RsLCD, SysParaSet_Handler);
  RsLCD.reg_hdle_stat = SYSPARADISP_REG_START;
  RsLCD.reg_hdle_end = SYSPARADISP_REG_END;
  RsRegHdle(&RsLCD, SysParaDisp_Handler);
  RsLCD.reg_hdle_stat = DATAREAD_REG_START;
  RsLCD.reg_hdle_end = DATAREAD_REG_END;
  RsRegHdle(&RsLCD, DataRead_Handler);
  RsLCD.reg_hdle_stat = SENS_CARD_DATAREAD_REG_START;
  RsLCD.reg_hdle_end = SENS_CARD_REG_END;
  RsRegHdle(&RsLCD, SideCar_Sens_DataRead_Handler);
  RsLCD.reg_hdle_stat = DEVCTRL_REG_START;
  RsLCD.reg_hdle_end = DEVCTRL_REG_END;
  RsRegHdle(&RsLCD, DevCtrl_Handler);
  RsLCD.reg_hdle_stat = SENS_CARD_DEVCTRL_REG_START;
  RsLCD.reg_hdle_end = SENS_CARD_DEVCTRL_REG_END;
  RsRegHdle(&RsLCD, SideCar_Sens_DevCtrl_Handler);
  RsLCD.reg_hdle_stat = FANS_CARD_REG_START;
  RsLCD.reg_hdle_end = FANS_CARD_REG_END;
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
        // log_i("RS485 Read Success");
        // elog_hexdump("UPPER_rx_Data", 32, RsLCD.rx_Data, sizeof(RsLCD.rx_Data)/2);
      }

      xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);
      err = RS485ReadHandler(&RsLCD);
      xSemaphoreGive(RS485RegionMutex);

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
